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

	//start blink
	for(int i=0; i<5; i++)
	{
		ledOn();
		Thread::sleep(100);
		ledOff();
		Thread::sleep(100);
	}

	Button<GPIOA_BASE,0> button;

	//main loop
    while(true){
		button.check();

		// LedMatrix_Driver::setLed(6,5);
		// LedMatrix_Driver::setLed(0,8);
		// unsigned short x[LED_MATRIX_ROWS] {0,0,1,0,0,0,0,0,1,0};
		// unsigned short y[LED_MATRIX_COLUMNS] {0,1,1,0,0,0,0,0,0,0,0,0,1,0,0};
		// LedMatrix_Driver::setLeds(x, y);
			
    }
}