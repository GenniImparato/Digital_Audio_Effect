#ifndef AUDIO_EFFECTS__H__
#define AUDIO_EFFECTS__H__

#include <miosix.h>
#include "../config.h"


#define OUT_BUFFER_MAX 			65535
#define OUT_BUFFER_MIN			0
#define OUT_BUFFER_CENTER		((OUT_BUFFER_MAX-OUT_BUFFER_MIN)/2)

//base class for audio effect
//runs on a its own thread
class AudioEffect
{
	public:
		//starts a new thread
		AudioEffect();
		//terminates thread
		virtual ~AudioEffect();

	//override to implement different effects
	protected:
		virtual void		preWrite(){};
		virtual void		writeNextBuffer(unsigned short* wrBuff);
		virtual void		postWrite();

	private:
		miosix::Thread*		thread;

		//effect main loop
		virtual void		loop();
		//thread entry point
		static  void		threadMain(void *param);

		//default effect vars
		int 				tim = 0;
		int 				tim2 = 0;
		bool 				rising = true;
		bool 				rising2 = true;
		float  				envelope = 0;
};


#endif
