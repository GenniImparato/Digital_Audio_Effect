#include "ADC_Driver.h"

using namespace miosix;

const unsigned short    ADC_Driver::potChannels[POTS_COUNT] = {2,0,3,1};

AudioBufferQueue*       ADC_Driver::dmaBuffer;
miosix::Thread*         ADC_Driver::waitingThread;

unsigned short ADC_Driver::buffer[AUDIO_BUFFERS_SIZE];


typedef Gpio<GPIOA_BASE,2> pot0;
typedef Gpio<GPIOA_BASE,0> pot1;
typedef Gpio<GPIOA_BASE,3> pot2;
typedef Gpio<GPIOA_BASE,1> pot3;
typedef Gpio<GPIOC_BASE,4> audioInput;

void ADC_Driver::init()
{
    dmaBuffer = new AudioBufferQueue();

    pot0::mode(Mode::INPUT_ANALOG);
    pot1::mode(Mode::INPUT_ANALOG);
    pot2::mode(Mode::INPUT_ANALOG);
    pot3::mode(Mode::INPUT_ANALOG);
    audioInput::mode(Mode::INPUT_ANALOG);

    {
        FastInterruptDisableLock dLock;
        //Enable DMA2 clock
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
        RCC_SYNC();
    }
	//enable ADC1 clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC_SYNC();

   	//activate ADC1
    ADC1->CR2 = ADC_CR2_ADON;

    configureADC2();

    //wait
    miosix::delayMs(500);

    NVIC_SetPriority(DMA2_Stream2_IRQn,2);//High priority for DMA
    NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    dmaRestart();

    /*unsigned short *buff;
    unsigned int size;
    dmaBuffer->tryGetWritableBuffer(buff);
   
    for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
    {
        buff[i] = i*60000/AUDIO_BUFFERS_SIZE;
    }
    dmaBuffer->bufferFilled(AUDIO_BUFFERS_SIZE);*/

    configureTIM2();
}

void ADC_Driver::configureTIM2(void)
{
    // enable TIM2 clock (bit0)
    RCC->APB1ENR |= (1 << 0);           

    //44.1 Khz
    TIM2->PSC = 15;
    TIM2->ARR = 59;

    /* Reset the MMS Bits */
    TIM2->CR2 &= (uint16_t)~TIM_CR2_MMS;
    /* Select the TRGO source */
    TIM2->CR2 |=  TIM_CR2_MMS_1; //UPDATE EVENT       

    // Update Interrupt Enable
    //TIM2->DIER |= (1 << 0);

    // enable TIM2 IRQ from NVIC
    //NVIC_EnableIRQ(TIM2_IRQn);

    // Enable Timer 2 module (CEN, bit0)
    TIM2->CR1 |= (1 << 0);
}

void ADC_Driver::configureADC2()
{
    RCC->APB2ENR|=RCC_APB2ENR_ADC2EN;  //ADC2 clock enable
    RCC_SYNC();
    ADC2->SQR1&=~ADC_SQR1_L;                         
        
    //set number of conversions per sequence to 1
    ADC2->SQR3&=~ADC_SQR3_SQ1;                   
        
    //clear channel select bits
    ADC2->SQR3|=14;         //set channel
    ADC2->CR1 &= ~ADC_CR1_SCAN; //SCAN mode disabled    
    ADC2->CR2 &= ~ADC_CR2_CONT; //Disable continuous conversion
                    
    ADC2->SMPR1 |= ADC_SMPR1_SMP14_2 | ADC_SMPR1_SMP14_1;
    
    //Trigger on Rising edge
    ADC2->CR2 &=  ~(1<<29);
    ADC2->CR2 |= (1<<28);   
    
    //External Event TIM2 TRGO [0110]
    const uint32_t TIM2_TRGO = ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1; 
    ADC2->CR2 &= ~(ADC_CR2_EXTSEL);   // remove all selections
    ADC2->CR2 |= TIM2_TRGO;
    
    ADC2->CR2|=ADC_CR2_ADON;
 
    //enable DMA
    ADC2->CR2 |= ADC_CR2_DMA;  

    //starts ADC    
    ADC2->CR2|=ADC_CR2_SWSTART;     
}

//performs a single conversion (uses ADC1)
unsigned int ADC_Driver::singleConversion(unsigned short channel)
{
	//single conversion mode
	ADC1->CR1 = 0;

	//select sample time
    ADC1->SMPR1 = 	  ADC_SMPR1_SMP10_2
              		| ADC_SMPR1_SMP10_1
              		| ADC_SMPR1_SMP10_0; //239.5 cycles sample time

    //select number of converions
    ADC1->SQR1 = 0; 		//single conversion

    //select channel
    ADC1->SQR2 = 0;
    ADC1->SQR3 = channel; 		//channel 1

    //start conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;

    //wait for conversion to end
    while((ADC1->SR & ADC_SR_EOC)==0);

    unsigned int result=ADC1->DR;

    return result;
}

//performs a single conversion given pot id (uses ADC1)
unsigned int ADC_Driver::singleConversionPot(unsigned short pot)
{
	if(pot>=POTS_COUNT)
		return 0;

	return singleConversion(potChannels[pot]);
}

/**
 * Waits until a buffer is available for reading
 * \return a readable buffer from dmaBuffer
 */
const unsigned short * ADC_Driver::getReadableBuffer(miosix::Thread* readerThread)
{
    FastInterruptDisableLock dLock;
    waitingThread = readerThread;
    const unsigned short *result;
    unsigned int size;
    while(dmaBuffer->tryGetReadableBuffer(result, size)==false)
    {
        waitingThread->IRQwait();
        {
            FastInterruptEnableLock eLock(dLock);
            Thread::yield();
        }
    }
    return result;
}

//Configure the DMA to do another transfer
bool ADC_Driver::IRQdmaRestart()
{
    unsigned short* buffer;

    DMA2_Stream2->CR = 0;
    DMA2_Stream2->PAR=reinterpret_cast<unsigned int>(&ADC2->DR); 


    if(!dmaBuffer->tryGetWritableBuffer(buffer))
    {   
        //dmaRefillWaiting = true;
        //miosix::ledOn();
        return false;
    }
    else    
        DMA2_Stream2->M0AR=reinterpret_cast<unsigned int>(buffer);

    DMA2_Stream2->NDTR=AUDIO_BUFFERS_SIZE;
    DMA2_Stream2->CR=DMA_SxCR_PL_1    | //High priority DMA stream
                     DMA_SxCR_MSIZE_0 | //Read  16bit at a time from ADC2
                     DMA_SxCR_PSIZE_0 | //Write 16bit at a time to RAM
                     DMA_SxCR_MINC    | //Increment RAM pointer
                     DMA_SxCR_TCIE    | //Interrupt on completion
                     DMA_SxCR_CHSEL_0 | //Select channel 1
                     DMA_SxCR_EN;       //Start the DMA

    return true;
}

void ADC_Driver::bufferEmptied()
{
    FastInterruptDisableLock dLock;
    dmaBuffer->bufferEmptied();
}


//Helper function used to start a DMA transfer from non-interrupt code
bool ADC_Driver::dmaRestart()
{
    FastInterruptDisableLock dLock;
    return IRQdmaRestart();
}

//DMA end of transer interrupt handler
void ADC_Driver::IRQdmaEndHandler()
{
    DMA2->LIFCR=DMA_LIFCR_CTCIF2  |
                DMA_LIFCR_CTEIF2  |
                DMA_LIFCR_CDMEIF2 |
                DMA_LIFCR_CFEIF2;

    dmaBuffer->bufferFilled(AUDIO_BUFFERS_SIZE);

    ADC2->CR2 &= ~(ADC_CR2_DDS|ADC_CR2_DMA);

    //dmaBuffer->bufferEmptied();

    if(IRQdmaRestart())
    {
        //miosix::ledOn();
        ADC2->SR &= ~(ADC_SR_OVR);
        ADC2->CR2 |= (ADC_CR2_DMA|ADC_CR2_DDS);
        //clear the overload
        ADC2->SR &= ~(ADC_SR_OVR|ADC_SR_STRT);
        //start the ADC
        ADC2->CR2 |= (uint32_t)ADC_CR2_SWSTART;

        waitingThread->IRQwakeup();
        if(waitingThread->IRQgetPriority()>Thread::IRQgetCurrentThread()->IRQgetPriority())
            Scheduler::IRQfindNextThread();
    }
    //else
        //miosix::ledOff();

    //dmaBuffer->bufferEmptied();

}

void ADC_Driver::IRQTIM2Handler(void)
{  
    // clear interrupt status
    if (TIM2->DIER & 0x01) 
    {
        if (TIM2->SR & 0x01) 
        {
            TIM2->SR &= ~(1U << 0);
        }
    }

    /*static bool ledState = false;
    static int count = 0;

    if(count >= 44100)
    {
        count = 0;
        ledState = !ledState;
    }
    else
        count++;

    ledState = !ledState;

    if(ledState)
        miosix::ledOn();
    else
        miosix::ledOff();*/
}

/**
 * DMA end of transfer interrupt
 */
void __attribute__((naked)) DMA2_Stream2_IRQHandler()
{
    saveContext();
    asm volatile("bl _Z24DMA2_Stream2_IRQHandler_v");
    restoreContext();
}

/**
 * DMA end of transfer interrupt actual implementation
 */
void __attribute__((used)) DMA2_Stream2_IRQHandler_()
{
    ADC_Driver::IRQdmaEndHandler();
}

/**
 * Timer 2 interrupt
 */
void __attribute__((naked)) TIM2_IRQHandler()
{
    saveContext();
    asm volatile("bl _Z16TIM2_IRQHandler_v");
    restoreContext();
}

/**
 * Timer 2 interrupt actual implementation
 */
void __attribute__((used)) TIM2_IRQHandler_()
{
    ADC_Driver::IRQTIM2Handler();
}

