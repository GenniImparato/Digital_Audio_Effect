#include <miosix.h>
#include <mutex>
#include "config.h"
#include "AudioChain.h"
#include "Button.h"
#include "ADC_Driver/ADC_Driver.h"
#include "DAC_Driver/DAC_Driver.h"
#include "LedMatrix_Driver/LedMatrix_Driver.h"

using namespace miosix;

int main()
{
	LedMatrix_Driver::init();

	AudioChain::init();

	ADC_Driver::init();
	DAC_Driver::init();
	DAC_Driver::setVolume(0);
	
	AudioChain::startThreads();


	Button<GPIOA_BASE,0> button;
	//main loop
    while(true)
    {
		button.check();	
		Thread::sleep(BUTTONS_REFRESH_PERIOD);
    }
}