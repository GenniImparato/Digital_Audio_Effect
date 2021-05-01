#ifndef AUDIO_EFFECT_DISTORSION___H___
#define AUDIO_EFFECT_DISTORSION___H___

#include "AudioEffect.h"

#define DELAY_SAMPLES_COUNT	AUDIO_BUFFERS_SIZE*45		 	

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
		int					delayIndex = 0;

		unsigned short		delayBuff[DELAY_SAMPLES_COUNT];

};

#endif