#ifndef ADC_DRIVER___H__
#define ADC_DRIVER___H__

#include <miosix.h>
#include "../config.h"

#define POTS_COUNT				4

//static class to manage ADC
class ADC_Driver
{
	private:
		//private contructor, ADC_Driver has only static members
		ADC_Driver() {};

		//potentiometers/channels map
		static const unsigned short potChannels[POTS_COUNT];

	public:

		//initialize ADC
		static void		 		init();
		
		//performs a single conversion
		static unsigned int 	singleConversion(unsigned short channel);

		//performs a single conversion given pot id
		static unsigned int 	singleConversionPot(unsigned short pot);
};

#endif