#include "AudioEffect.h"
#include "../DAC_Driver/DAC_Driver.h"

using namespace miosix;

AudioEffect::AudioEffect()
{
}

AudioEffect::~AudioEffect()
{
	thread->terminate();

	//wait for thread termination before deleting object
	//fix crash on delete
	while(Thread::exists(thread))
		Thread::yield();
}

void AudioEffect::startThread()
{
	thread = Thread::create(&AudioEffect::threadMain, 256, 1, this);
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

void AudioEffect::loop()
{
	preWrite();						//virtual method

	//Write next samples on DAC
	unsigned short* wrBuff = DAC_Driver::getWritableBuffer(thread); 
	writeNextBuffer(wrBuff);		//virtual method
	DAC_Driver::bufferFilled();

	postWrite();					//virtual method
}

void AudioEffect::threadMain(void *param)
{
	while(!Thread::testTerminate())
	{
		((AudioEffect*)param)->loop();
	}
}