#ifndef AUDIO_EFFECT_DISTORSION___H___
#define AUDIO_EFFECT_DISTORSION___H___

#include "AudioEffect.h"

#define DELAY_SAMPLES_COUNT	AUDIO_BUFFERS_SIZE*30		 	

class Distorsion	:public AudioEffect
{
	public:
		Distorsion();
		~Distorsion(){};

		//overriden methods
		void				writeNextBuffer(float* inBuff, float* outBuff);
		void				postWrite();

	private:
		float				speed		= 1.0;
		float				volume		= 1.0;
};

#endif