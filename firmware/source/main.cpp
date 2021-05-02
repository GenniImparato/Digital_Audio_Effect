#include <miosix.h>
#include <mutex>
#include "config.h"
#include "AudioChain.h"
#include "ADC_Driver/ADC_Driver.h"
#include "DAC_Driver/DAC_Driver.h"
#include "LedMatrix_Driver/LedMatrix_Driver.h"


using namespace miosix;

//AudioEffect			*effect;
miosix::Mutex 	mutex;


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
	for(int i=0; i<5; i++)
	{
		ledOn();
		Thread::sleep(100);
		ledOff();
		Thread::sleep(100);
	}

	//main loop
    while(true)
    {
		{
			std::unique_lock<miosix::Mutex> lock(mutex);
			LedMatrix_Driver::emptyBuffer();
			LedMatrix_Driver::setString("bella");
		}
		Thread::sleep(1000);
		{
			std::unique_lock<miosix::Mutex> lock(mutex);
			LedMatrix_Driver::emptyBuffer();
			LedMatrix_Driver::setString("genni");
		}
		Thread::sleep(1000);
		{
			std::unique_lock<miosix::Mutex> lock(mutex);
			LedMatrix_Driver::emptyBuffer();
			LedMatrix_Driver::setString("come");
		}
		Thread::sleep(1000);
		{
			std::unique_lock<miosix::Mutex> lock(mutex);
			LedMatrix_Driver::emptyBuffer();
			LedMatrix_Driver::setString("va");
		}
		Thread::sleep(1000);
    }
}