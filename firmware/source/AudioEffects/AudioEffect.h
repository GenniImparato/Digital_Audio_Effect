#ifndef AUDIO_EFFECTS__H__
#define AUDIO_EFFECTS__H__

#include <miosix.h>
#include "../config.h"
#include "../utils.h"


//base class for audio effect
//runs on a its own thread
class AudioEffect
{
	public:
		AudioEffect();
		//terminates threads
		virtual ~AudioEffect();

		unsigned int 			ctrlValues[POTS_COUNT];

		virtual void			preWrite(){};
		virtual void			writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer);
		virtual void			postWrite();


	private:
};


#endif
