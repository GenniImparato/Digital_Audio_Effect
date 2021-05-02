#include "Delay.h"
#include <math.h>       /* exp */


Delay::Delay()
{
	for(int i=0; i<DELAY_SAMPLES_COUNT; i++)
		delayBuff[i] = 0;
	delayIndex=0;
}

void Delay::writeNextBuffer(float* inBuff, float* outBuff)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	{
		float sample = inBuff[i]*0.5 + delayBuff[delayIndex]*0.5;
		outBuff[i] = sample;

		float delaySample = (inBuff[i] + delayBuff[delayIndex])*feedback;
		delayBuff[delayIndex] = delaySample;

		delayIndex++;

		if(delayIndex >= time*DELAY_SAMPLES_COUNT)
			delayIndex=0;
	}

}

void Delay::postWrite()
{
	time = controls[0].getFloatValue(0.1, 1.0);
	feedback = controls[1].getFloatValue(0, 1.0);
	wet = controls[2].getFloatValue(0, 1);
}



