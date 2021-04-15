#include "ADC_Driver.h"

using namespace miosix;

const unsigned short ADC_Driver::potChannels[POTS_COUNT] = {2,0,3,1};

void ADC_Driver::init()
{
	//enable clock
	RCC->APB2ENR |= (1<<8);
    RCC_SYNC();

   	//activate ADC
    ADC1->CR2 = ADC_CR2_ADON;

    //wait
    miosix::delayMs(500);

    typedef Gpio<GPIOA_BASE,0> A0;
    typedef Gpio<GPIOA_BASE,1> A1;
    typedef Gpio<GPIOA_BASE,2> A2;
    typedef Gpio<GPIOA_BASE,3> A3;
	A0::mode(Mode::INPUT_ANALOG);
	A1::mode(Mode::INPUT_ANALOG);
	A2::mode(Mode::INPUT_ANALOG);
	A3::mode(Mode::INPUT_ANALOG);
}


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

unsigned int ADC_Driver::singleConversionPot(unsigned short pot)
{
	if(pot>=POTS_COUNT)
		return 0;

	return singleConversion(potChannels[pot]);
}

