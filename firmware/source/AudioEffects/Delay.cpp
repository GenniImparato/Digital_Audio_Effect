#include "Delay.h"
#include <math.h>       /* exp */


Delay::Delay()
{
	for(int i=0; i<DELAY_SAMPLES_COUNT; i++)
		delayBuff[i] = 0;
}

void Delay::writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	{
		float sample = rdBuffer[i]*0.5 + delayBuff[delayIndex]*0.5;
		wrBuff[i] = sample;

		float delaySample = (wrBuff[i] + delayBuff[delayIndex])*0.45;
		delayBuff[delayIndex] = delaySample;

		delayIndex++;
		if(delayIndex >= DELAY_SAMPLES_COUNT)
			delayIndex=0;
	}

}

void Delay::postWrite()
{

}



