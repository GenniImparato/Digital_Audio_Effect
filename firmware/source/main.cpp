#include <miosix.h>
#include "config.h"
#include "ADC_Driver/ADC_Driver.h"
#include "DAC_Driver/DAC_Driver.h"
#include "LedMatrix_Driver/LedMatrix_Driver.h"
#include "AudioEffects/AudioEffect.h"
#include "AudioEffects/Synthesizer.h"

using namespace miosix;

typedef Gpio<GPIOA_BASE,1> analogIn;

AudioEffect			*effect;


int main()
{
	analogIn::mode(Mode::INPUT_ANALOG);

	effect = new Synthesizer();

	LedMatrix_Driver::init();
	ADC_Driver::init();
	DAC_Driver::init();
	DAC_Driver::setVolume(-10);

	//starts new thread
	effect->startThreads();

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
		ledOn();
		Thread::sleep(1000);
		ledOff();
		Thread::sleep(1000);
    }
}