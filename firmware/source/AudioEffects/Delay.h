#ifndef AUDIO_EFFECT_DISTORSION___H___
#define AUDIO_EFFECT_DISTORSION___H___

#include "AudioEffect.h"

#define DELAY_SAMPLES_COUNT	AUDIO_BUFFERS_SIZE*40		 	

class Delay	:public AudioEffect
{
	public:
		Delay();
		~Delay(){};

		//overriden methods
		void				writeNextBuffer(float* inBuff, float* outBuff);
		void				postWrite();

	private:
		int					delayIndex 	= 0;
		float				time		= 0.5;
		float				feedback    = 0.45;
		float 				cut 		= 1.0;

		float				delayBuff[DELAY_SAMPLES_COUNT];

};

#endif