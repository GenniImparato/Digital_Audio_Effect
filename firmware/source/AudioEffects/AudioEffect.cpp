#include "AudioEffect.h"
#include "../DAC_Driver/DAC_Driver.h"

void AudioEffect::writeNextBuffer(unsigned short* wrBuff)
{
	//generate sawtooth wave
	for(int i=0;i<AUDIO_BUFFERS_SIZE;i++)
				wrBuff[i]= envelope*i*(6000/AUDIO_BUFFERS_SIZE);
	DAC_Driver::bufferFilled();

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
	unsigned short* wrBuff = DAC_Driver::getWritableBuffer(); 	//waits for buffer, suspends thread if not available
	writeNextBuffer(wrBuff);
}