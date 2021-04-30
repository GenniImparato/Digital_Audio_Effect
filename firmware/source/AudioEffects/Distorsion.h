#ifndef AUDIO_EFFECT_DISTORSION___H___
#define AUDIO_EFFECT_DISTORSION___H___

#include "AudioEffect.h"


class Distorsion	:public AudioEffect
{
	public:
		Distorsion(){};
		~Distorsion(){};

		//overriden methods
		void				preWrite();
		void				writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer);
		void				postWrite(){};

	private:
		float				topThresh = 0.8;
		float				botThresh = 0.2;
		float				dist=0.1;

};

#endif