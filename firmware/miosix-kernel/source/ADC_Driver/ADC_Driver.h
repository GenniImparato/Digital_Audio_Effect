#include <miosix.h>

namespace ADC_Driver
{

	//initialize ADC
	void		 enable();
	
	//performs a single conversion
	unsigned int singleConversion(int channel);
}