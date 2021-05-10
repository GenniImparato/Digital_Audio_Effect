#include "Delay.h"
#include <math.h>       /* exp */

//float Delay::delayBuff[DELAY_SAMPLES_COUNT] = {};

Delay::Delay()
{
	for(int i=0; i<DELAY_SAMPLES_COUNT; i++)
		delayBuff[i] = 0;

	delayIndex=0;

	controls[0].setName(std::string("TIME "));
	controls[0].setFloatDigits(2);
	controls[1].setName(std::string("FEDBK"));
	controls[1].setFloatDigits(2);
	controls[2].setName(std::string("CUT  "));
	controls[2].setFloatDigits(2);

	setName(std::string("DELAY"));
}

void Delay::writeNextBuffer(float* inBuff, float* outBuff)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	{

		if(delayIndex>=0)
		{
			outBuff[i] = inBuff[i]*0.5 + delayBuff[delayIndex]*0.5;
			delayBuff[delayIndex] = inBuff[i] + delayBuff[delayIndex]*feedback;
		}
		else
		{
			outBuff[i] = inBuff[i]*0.5 + delayBuff[i]*0.5;
			delayBuff[i] = inBuff[i]*feedback;
		}

		delayIndex++;

		if(delayIndex >= time*DELAY_SAMPLES_COUNT)
			delayIndex=-cut*DELAY_SAMPLES_COUNT;
	}

}

void Delay::postWrite()
{
	time = controls[0].getFloatValue(0.1, 1.0);
	feedback = controls[1].getFloatValue(0, 0.6);
	cut = controls[2].getFloatValue(0, 1);
}



