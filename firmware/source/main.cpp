#include <miosix.h>
#include "config.h"
#include "AudioChain.h"
#include "ADC_Driver/ADC_Driver.h"
#include "DAC_Driver/DAC_Driver.h"
#include "LedMatrix_Driver/LedMatrix_Driver.h"

using namespace miosix;

//AudioEffect			*effect;


int main()
{
	//effect = new Distorsion();

	LedMatrix_Driver::init();

	AudioChain::init();

	ADC_Driver::init();
	DAC_Driver::init();
	DAC_Driver::setVolume(0);

	
	AudioChain::startThreads();

	//start blink
	for(int i=0; i<35; i++)
	{
		ledOn();
		Thread::sleep(100);
		ledOff();
		Thread::sleep(100);
	}

	//main loop
    while(true)
    {
		ledOff();
		Thread::sleep(1000);
		ledOff();
		Thread::sleep(1000);
    }
}