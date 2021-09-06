#include "Distorsion.h"
#include <math.h>       /* exp */

//float Delay::delayBuff[DELAY_SAMPLES_COUNT] = {};

Distorsion::Distorsion()
{
	controls[0].setName(std::string("SPEED"));
	controls[0].setFloatDigits(2);

	setName(std::string("TREML"));
}

void Distorsion::writeNextBuffer(float* inBuff, float* outBuff)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	{
		outBuff[i] = inBuff[i]*volume;
	}
}

void Distorsion::postWrite()
{
	speed = controls[0].getFloatValue(0.1, 1.0);

	static bool rising = false;

	if(rising)
	{
		volume+=1/(100*speed);
		if(volume >= 1.0)
		{
			volume = 1.0;
			rising=false;
		}
	}
	else
	{
		volume-=1/(100*speed);
		if(volume <= 0.0)
		{
			volume = 0.0;
			rising=true;
		}
	}
	

}

