#ifndef ADC_DRIVER___H__
#define ADC_DRIVER___H__

#include <miosix.h>
#include "../config.h"


namespace ADC_Driver
{

	//initialize ADC
	void		 init();
	
	//performs a single conversion
	unsigned int singleConversion(int channel);
}

#endif