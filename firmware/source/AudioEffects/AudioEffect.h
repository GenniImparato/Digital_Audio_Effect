#ifndef AUDIO_EFFECTS__H__
#define AUDIO_EFFECTS__H__

#include <miosix.h>
#include "../config.h"


//base class for audio effect
//base implementation reads audio samples from ADC and writes output samples on DAC 
class AudioEffect
{
	public:
		AudioEffect(){};

		//must be override
		//should generate out samples
		virtual void	writeNextBuffer(unsigned short* wrBuff);

		void			loop();

		//writes next 256 samples to outBuffer
		void 			writeOutBuffer();

	private:

	int tim = 0;
	int tim2 = 0;
	bool rising = true;
	bool rising2 = true;
	float  envelope = 0;
};


#endif
