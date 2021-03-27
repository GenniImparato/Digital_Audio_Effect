#include <miosix.h>
#include "config.h"
#include "ADC_Driver/ADC_Driver.h"
#include "DAC_Driver/DAC_Driver.h"
#include "AudioEffects/AudioEffect.h"

using namespace miosix;

typedef Gpio<GPIOA_BASE,1> analogIn;


AudioBufferQueue	inBuffer;
AudioBufferQueue 	outBuffer;

AudioEffect			*effect;


int main()
{
	//DSP::Handler::instance().initPeripherals();
	analogIn::mode(Mode::INPUT_ANALOG);

	ADC_Driver::init();
	DAC_Driver::init();
	DAC_Driver::setVolume(0);

	effect = new AudioEffect();

	//start blink
	/*for(int i=0; i<5; i++)
	{
		ledOn();
		Thread::sleep(250);
		ledOff();
		Thread::sleep(250);
	}*/

	//main loop
    while(true)
    {
		ledOn();
		Thread::sleep(250);
		ledOff();
		Thread::sleep(250);
    }
}