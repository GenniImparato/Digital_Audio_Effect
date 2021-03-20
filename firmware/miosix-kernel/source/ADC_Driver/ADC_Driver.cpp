#include "ADC_Driver.h"

namespace ADC_Driver
{
	void enable()
	{
		//enable clock
		RCC->APB2ENR |= (1<<8);
        RCC_SYNC();

       	//activate ADC
	    ADC1->CR2 = ADC_CR2_ADON;

	    //wait
	    miosix::delayMs(500);
	}

	unsigned int singleConversion(int channel)
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
}
