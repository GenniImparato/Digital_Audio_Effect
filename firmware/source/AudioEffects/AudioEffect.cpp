#include "AudioEffect.h"
#include "../DAC_Driver/DAC_Driver.h"
#include "../ADC_Driver/ADC_Driver.h"

using namespace miosix;

Mutex 	AudioEffect::ctrlMutex;

AudioEffect::AudioEffect()
{
}

AudioEffect::~AudioEffect()
{
	dspThread->terminate();
	potThread->terminate();

	//wait for thread termination before deleting object
	//fix crash on delete
	while(Thread::exists(dspThread) && Thread::exists(potThread))
		Thread::yield();
}

void AudioEffect::startThreads()
{
	dspThread = Thread::create(&AudioEffect::dspThreadMain, 2048, 1, this);
	potThread = Thread::create(&AudioEffect::potThreadMain, STACK_MIN, 0, this);
}


void AudioEffect::writeNextBuffer(unsigned short* wrBuff)
{
	//generate sawtooth wave with tremolo
	for(int i=0;i<AUDIO_BUFFERS_SIZE;i++)
				wrBuff[i]= envelope*i*(6000/AUDIO_BUFFERS_SIZE);
}

void AudioEffect::postWrite()
{
	tim++;

	tim = 0;
	if(rising)
	{
		envelope+=0.01;
	}
	else
	{
		envelope-=0.01;
	}

	if(envelope>=1.0)
		rising = false;
	else if (envelope<=0.0)
		rising = true;
}

void AudioEffect::dspLoop()
{
	preWrite();						//virtual method

	//Write next samples on DAC
	unsigned short* wrBuff = DAC_Driver::getWritableBuffer(dspThread); 
	writeNextBuffer(wrBuff);		//virtual method
	DAC_Driver::bufferFilled();

	postWrite();					//virtual method
}

void AudioEffect::potLoop()
{
	{
		Lock<Mutex> lock(ctrlMutex);
		for(int i=0; i<CONTROLS_COUNT; i++)
			ctrlValues[i] = ADC_Driver::singleConversionPot(i);
	}

	Thread::sleep(CONTROLS_REFRESH_PERIOD);
}

void AudioEffect::dspThreadMain(void *param)
{
	while(!Thread::testTerminate())
	{
		((AudioEffect*)param)->dspLoop();
	}
}

void AudioEffect::potThreadMain(void *param)
{
	while(!Thread::testTerminate())
	{
		((AudioEffect*)param)->potLoop();
	}
}