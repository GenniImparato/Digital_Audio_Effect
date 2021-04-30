#ifndef AUDIO_EFFECT_DISTORSION___H___
#define AUDIO_EFFECT_DISTORSION___H___

#include "AudioEffect.h"


class Delay	:public AudioEffect
{
	public:
		Delay();
		~Delay(){};

		//overriden methods
		void				preWrite(){};
		void				writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer);
		void				postWrite();

	private:
		float				topThresh = 0.8;
		float				botThresh = 0.2;
		float				dist=0.1;

		int					delayIndex = 0;

		unsigned short		delayBuff[AUDIO_BUFFERS_SIZE*40];

};

#endif