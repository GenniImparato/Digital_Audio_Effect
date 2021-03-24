#include <miosix.h>
#include "config.h"
#include "ADC_Driver/ADC_Driver.h"
#include "DAC_Driver/DAC_Driver.h"
#include "DSP/DSPBase.h"

using namespace miosix;

typedef Gpio<GPIOA_BASE,1> analogIn;


AudioBufferQueue	inBuffer;
AudioBufferQueue 	outBuffer;


int main()
{
	//DSP::Handler::instance().initPeripherals();
	analogIn::mode(Mode::INPUT_ANALOG);

	ADC_Driver::init();
	DAC_Driver::init();

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
    	/*unsigned int potValue = ADC_Driver::singleConversion(1);

    	if(potValue > 2000)
    		ledOn();
    	else
    		ledOff();*/

    	//Thread::sleep(500);

    	ledOn();
    	for(int i=0; i<1000; i++)
    	{
		    unsigned short* wrBuff = DAC_Driver::getWritableBuffer();
			for(int i=0;i<AUDIO_BUFFERS_SIZE;i++)
				wrBuff[i]= i*(6000/AUDIO_BUFFERS_SIZE);
			DAC_Driver::bufferFilled();
		}
		ledOff();

		Thread::sleep(500);
    }
}