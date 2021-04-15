#include <miosix.h>
#include "config.h"
#include "ADC_Driver/ADC_Driver.h"
#include "DAC_Driver/DAC_Driver.h"
#include "AudioEffects/AudioEffect.h"
#include "AudioEffects/Synthesizer.h"

using namespace miosix;

typedef Gpio<GPIOA_BASE,1> analogIn;
typedef Gpio<GPIOD_BASE,11> row1;
typedef Gpio<GPIOD_BASE,9> row3;
typedef Gpio<GPIOD_BASE,7> col7;

AudioEffect			*effect;


int main()
{
	analogIn::mode(Mode::INPUT_ANALOG);
	row1::mode(Mode::OUTPUT);
	row3::mode(Mode::OUTPUT);
	col7::mode(Mode::OUTPUT);

	effect = new Synthesizer();

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
		// ledOn();
		// Thread::sleep(1000);
		// ledOff();
		// Thread::sleep(1000);
		row1::low();
		col7::low();
		row3::high();

    }
}