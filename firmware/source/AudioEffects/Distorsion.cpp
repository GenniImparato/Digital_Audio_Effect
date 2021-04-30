#include "Distorsion.h"
#include <math.h>       /* exp */



void Distorsion::writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	{
		float fInSample = (float)(rdBuffer[i])/(float)(OUT_BUFFER_MAX);
		float fOutSample = fInSample;

		if (fInSample > topThresh)
		   fOutSample = 1.0 - dist*exp(-fInSample);
		else if(fInSample < botThresh)
		   fOutSample = dist*exp(fInSample);
		
		wrBuff[i] = fOutSample*OUT_BUFFER_MAX;
	}
}

void Distorsion::preWrite()
{
	dist 	=   map_f(ctrlValues[0], 1, 4096, 0.1, 2.5);
	botThresh = map_f(ctrlValues[1], 1, 4096, 0, 0.5);
	topThresh = map_f(ctrlValues[2], 1, 4096, 0.5, 1);
}

