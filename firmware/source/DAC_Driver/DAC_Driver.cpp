#include "DAC_Driver.h"

using namespace miosix;

typedef Gpio<GPIOB_BASE,6>  scl;
typedef Gpio<GPIOB_BASE,9>  sda;
typedef Gpio<GPIOA_BASE,4>  lrck;
typedef Gpio<GPIOC_BASE,7>  mclk;
typedef Gpio<GPIOC_BASE,10> sclk;
typedef Gpio<GPIOC_BASE,12> sdin;
typedef Gpio<GPIOD_BASE,4>  reset;
typedef SoftwareI2C<sda,scl> i2c;

AudioBufferQueue*	DAC_Driver::dmaBuffer;
unsigned short*		DAC_Driver::dmaZeroBuffer;
Thread*				DAC_Driver::waitingThread;

/**
 * Waits until a buffer is available for writing
 * \return a writable buffer from dmaBuffer
 */
unsigned short * DAC_Driver::getWritableBuffer(miosix::Thread* writerThread)
{
	FastInterruptDisableLock dLock;
	waitingThread = writerThread;
	unsigned short *result;
	while(dmaBuffer->tryGetWritableBuffer(result)==false)
	{
		waitingThread->IRQwait();
		{
			FastInterruptEnableLock eLock(dLock);
			Thread::yield();
		}
	}
	return result;
}

void DAC_Driver::bufferFilled()
{
	FastInterruptDisableLock dLock;
	dmaBuffer->bufferFilled(dmaBuffer->bufferMaxSize());
}

//Configure the DMA to do another transfer
void DAC_Driver::IRQdmaRefill()
{
    const unsigned short *buffer;
	unsigned int size;

    DMA1_Stream5->CR=0;
	DMA1_Stream5->PAR=reinterpret_cast<unsigned int>(&SPI3->DR);

	//if no buffer available, uses zero buff for dma
	if(!dmaBuffer->tryGetReadableBuffer(buffer,size))
		DMA1_Stream5->M0AR=reinterpret_cast<unsigned int>(dmaZeroBuffer);
	else	
		DMA1_Stream5->M0AR=reinterpret_cast<unsigned int>(buffer);

	DMA1_Stream5->NDTR=size;
	DMA1_Stream5->CR=DMA_SxCR_PL_1    | //High priority DMA stream
                     DMA_SxCR_MSIZE_0 | //Read  16bit at a time from RAM
					 DMA_SxCR_PSIZE_0 | //Write 16bit at a time to SPI
				     DMA_SxCR_MINC    | //Increment RAM pointer
			         DMA_SxCR_DIR_0   | //Memory to peripheral direction
			         DMA_SxCR_TCIE    | //Interrupt on completion
			  	     DMA_SxCR_EN;       //Start the DMA
}


//Helper function used to start a DMA transfer from non-interrupt code
void DAC_Driver::dmaRefill()
{
	FastInterruptDisableLock dLock;
	IRQdmaRefill();
}

//initialize DAC, DMA and GPIO
void DAC_Driver::init()
{
	dmaBuffer = new AudioBufferQueue();

	dmaZeroBuffer = new unsigned short[AUDIO_BUFFERS_SIZE];
	for(int i=0;i<AUDIO_BUFFERS_SIZE/2;i++)
		dmaZeroBuffer[i]=0;
	for(int i=AUDIO_BUFFERS_SIZE/2;i<AUDIO_BUFFERS_SIZE;i++)
		dmaZeroBuffer[i]=6000;

    {
        FastInterruptDisableLock dLock;
        //Enable DMA1 and SPI3/I2S3
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
        RCC_SYNC();
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
        RCC_SYNC();
        //Configure GPIOs
        i2c::init();
        mclk::mode(Mode::ALTERNATE);
        mclk::alternateFunction(6);
        sclk::mode(Mode::ALTERNATE);
        sclk::alternateFunction(6);
        sdin::mode(Mode::ALTERNATE);
        sdin::alternateFunction(6);
        lrck::mode(Mode::ALTERNATE);
        lrck::alternateFunction(6);
        reset::mode(Mode::OUTPUT);
        //Enable audio PLL (settings for 44100Hz audio)
        RCC->PLLI2SCFGR=(2<<28) | (271<<6);
        RCC->CR |= RCC_CR_PLLI2SON;
    }
    //Wait for PLL to lock
    while((RCC->CR & RCC_CR_PLLI2SRDY)==0) ;
    
    reset::low(); //Keep in reset state
    delayUs(5);
    reset::high();
    delayUs(5);
    send(0x00,0x99); //These five command are the "magic" initialization
    send(0x47,0x80);
    send(0x32,0xbb);
    send(0x32,0x3b);
    send(0x00,0x00);
    
    send(0x05,0x20); //AUTO=0, SPEED=01, 32K=0, VIDEO=0, RATIO=0, MCLK=0
    send(0x04,0xaf); //Headphone always ON, Speaker always OFF
    send(0x06,0x04); //I2S Mode
    setVolume(-20);
    
    SPI3->CR2=SPI_CR2_TXDMAEN;
    SPI3->I2SPR=  SPI_I2SPR_MCKOE | 6;
	SPI3->I2SCFGR=SPI_I2SCFGR_I2SMOD    //I2S mode selected
                | SPI_I2SCFGR_I2SE      //I2S Enabled
                | SPI_I2SCFGR_I2SCFG_1; //Master transmit

    NVIC_SetPriority(DMA1_Stream5_IRQn,2);//High priority for DMA
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);    

    
    //set zero buffer
    unsigned short* wrBuff = getWritableBuffer(Thread::IRQgetCurrentThread());
	for(int i=0;i<AUDIO_BUFFERS_SIZE;i++)
		wrBuff[i]=0;
	bufferFilled();
    
	//Start playing
	dmaRefill();
	send(0x02,0x9e);
}

void DAC_Driver::send(unsigned char index, unsigned char data)
{
    i2c::sendStart();
    i2c::send(0x94);
    i2c::send(index);
    i2c::send(data);
    i2c::sendStop();
}

void DAC_Driver::setVolume(int db)
{
    db=2*std::max(-102,std::min(0,db));
    unsigned char vol=static_cast<unsigned int>(db) & 0xff;
    send(0x20,vol);
    send(0x21,vol);
}


//DMA end of transer interrupt handler
void DAC_Driver::IRQdmaEndHandler()
{
	DMA1->HIFCR=DMA_HIFCR_CTCIF5  |
                DMA_HIFCR_CTEIF5  |
                DMA_HIFCR_CDMEIF5 |
                DMA_HIFCR_CFEIF5;
	dmaBuffer->bufferEmptied();
	IRQdmaRefill();
	waitingThread->IRQwakeup();
	if(waitingThread->IRQgetPriority()>Thread::IRQgetCurrentThread()->IRQgetPriority())
		Scheduler::IRQfindNextThread();
}

/**
 * DMA end of transfer interrupt
 */
void __attribute__((naked)) DMA1_Stream5_IRQHandler()
{
    saveContext();
	asm volatile("bl _Z17I2SdmaHandlerImplv");
	restoreContext();
}

/**
 * DMA end of transfer interrupt actual implementation
 */
void __attribute__((used)) I2SdmaHandlerImpl()
{
	DAC_Driver::IRQdmaEndHandler();
}