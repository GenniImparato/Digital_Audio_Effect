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
	readADCThread->terminate();
	writeDACThread->terminate();
	potThread->terminate();

	//wait for thread termination before deleting object
	//fix crash on delete
	while(Thread::exists(readADCThread) && Thread::exists(writeDACThread) && Thread::exists(potThread))
		Thread::yield();
}

void AudioEffect::startThreads()
{
	readADCThread = Thread::create(&AudioEffect::readADCThreadMain, 2048, 3, this);
	writeDACThread = Thread::create(&AudioEffect::writeDACThreadMain, 2048, 3, this);
	potThread = Thread::create(&AudioEffect::potThreadMain, STACK_MIN, 0, this);
}


void AudioEffect::writeNextBuffer(unsigned short* wrBuff)
{
	//generate sawtooth wave with tremolo
	for(int i=0;i<AUDIO_BUFFERS_SIZE;i++)
				wrBuff[i]= tmpBuff[i];
}

void AudioEffect::postWrite()
{
}

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void AudioEffect::readADCLoop()
{
	//unsigned short tmpBuff[AUDIO_BUFFERS_SIZE];
	static int count = 0;

	const unsigned short* inBuff = ADC_Driver::getReadableBuffer(readADCThread);

	{	
		Lock<Mutex> dLock(buffMutex);
		for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
			tmpBuff[i] = map(inBuff[i], 1, 2000, 1, 60000);

	}


        /*printf("Buffer[%d] = %d\n", count, tmpBuff[100]);
        printf("Sample[%d] = %d\n", count, tmpBuff[400]);
    count++;*/
	ADC_Driver::bufferEmptied();
	//miosix::ledOn();
}

void AudioEffect::writeDACLoop()
{
	//const unsigned short* inBuff = ADC_Driver::getReadableBuffer(dspThread);
	/*for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
		tmpBuff[i] = inBuff[i];*/
	//for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
        //printf("Sample[%d] = %d\n", count, tmpBuff[100]);
        //printf("Sample[%d] = %d\n", count, inBuff[400]);
    //count++;
	//ADC_Driver::bufferEmptied();


	preWrite();					//virtual method*/

	//Write next samples on DAC
	unsigned short* outBuff = DAC_Driver::getWritableBuffer(writeDACThread); 
	/*for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
		outBuff[i] = tmpBuff[i];*/
	{
		Lock<Mutex> dLock(buffMutex);
		writeNextBuffer(outBuff);
	}
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

void AudioEffect::writeDACThreadMain(void *param)
{
	while(!Thread::testTerminate())
	{
		((AudioEffect*)param)->writeDACLoop();
	}
}

void AudioEffect::readADCThreadMain(void *param)
{
	while(!Thread::testTerminate())
	{
		((AudioEffect*)param)->readADCLoop();
	}
}

void AudioEffect::potThreadMain(void *param)
{
	while(!Thread::testTerminate())
	{
		((AudioEffect*)param)->potLoop();
	}
}