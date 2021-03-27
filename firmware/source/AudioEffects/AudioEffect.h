#ifndef AUDIO_EFFECTS__H__
#define AUDIO_EFFECTS__H__

#include <miosix.h>
#include "../config.h"


//base class for audio effect
//run on a thread
//base implementation reads audio samples from ADC and writes output samples on DAC 
class AudioEffect
{
	public:
		AudioEffect();

		//must be override
		//should generate out samples
		virtual void		writeNextBuffer(unsigned short* wrBuff);

		void				loop();

	private:
		miosix::Thread*		thread;
		static void			threadMain(void *param);

	int tim = 0;
	int tim2 = 0;
	bool rising = true;
	bool rising2 = true;
	float  envelope = 0;
};


#endif
