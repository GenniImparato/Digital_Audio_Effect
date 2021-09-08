#ifndef AUDIO_EFFECT_DISTORSION___H___
#define AUDIO_EFFECT_DISTORSION___H___

#include "AudioEffect.h"
	 	

class Tremolo	:public AudioEffect
{
	public:
		Tremolo();
		~Tremolo(){};

		//overriden methods
		void				writeNextBuffer(float* inBuff, float* outBuff);
		void				postWrite();

	private:
		float				speed		= 1.0;
		float				volume		= 1.0;
};

#endif