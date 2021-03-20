#include <miosix.h>
#include "ADC_Driver/ADC_Driver.h"

using namespace miosix;

typedef Gpio<GPIOA_BASE,1> analogIn;

int main()
{
	analogIn::mode(Mode::INPUT_ANALOG);
	ADC_Driver::enable();

	//start blink
	for(int i=0; i<5; i++)
	{
		ledOn();
		Thread::sleep(250);
		ledOff();
		Thread::sleep(250);
	}

	//main loop
    while(true)
    {
    	unsigned int potValue = ADC_Driver::singleConversion(1);

    	if(potValue > 2000)
    		ledOn();
    	else
    		ledOff();

    	Thread::sleep(100);
    }
}