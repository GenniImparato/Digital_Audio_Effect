#ifndef AUDIO_EFFECTS__H__
#define AUDIO_EFFECTS__H__

#include <miosix.h>
#include "../config.h"


#define OUT_BUFFER_MAX 				65535
#define OUT_BUFFER_MIN				0
#define OUT_BUFFER_CENTER			((OUT_BUFFER_MAX-OUT_BUFFER_MIN)/2)


#define CONTROLS_COUNT				4
#define CONTROLS_REFRESH_PERIOD		10 // ms

//base class for audio effect
//runs on a its own thread
class AudioEffect
{
	public:
		//doesn't start threads, need to call startThreads() after creation
		AudioEffect();
		//terminates threads
		virtual ~AudioEffect();

		void	startThreads();

	//override to implement different effects
	protected:
		virtual void			preWrite(){};
		virtual void			writeNextBuffer(unsigned short* wrBuff);
		virtual void			postWrite();

		unsigned int 			ctrlValues[CONTROLS_COUNT];


	private:
		//values read from pots
		
		static miosix::Mutex 	ctrlMutex;

		//main effect thread, high priority
		miosix::Thread*			dspThread;
		//periodic thread that reads potentiometers, low priority
		miosix::Thread*			potThread;

		//effect main dsp loop (not static)
		void					dspLoop();
		void					potLoop();

		//threads entry points, param is a pointer to AudioEffect
		static  void			dspThreadMain(void *param);
		static  void			potThreadMain(void *param);



		//---------------------------------------------------------------------
		//default effect vars
		int 				tim = 0;
		int 				tim2 = 0;
		bool 				rising = true;
		bool 				rising2 = true;
		float  				envelope = 0;
};


#endif
