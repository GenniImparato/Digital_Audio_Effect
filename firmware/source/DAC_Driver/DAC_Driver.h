#ifndef DAC_DRIVER___H__
#define DAC_DRIVER___H__

#include <miosix.h>
#include "../config.h"
#include "miosix/kernel/scheduler/scheduler.h"
#include "util/software_i2c.h"

//static class to manage DAC
class DAC_Driver
{
	private:
		//private contructor, DAC_Driver has only static members
		DAC_Driver() {};

		static AudioBufferQueue*	dmaBuffer;
		static miosix::Thread*		waitingThread;
		static bool					dmaRefillWaiting;

		//DAC main thread loop
		//static miosix::Thread*		thread;
		//static void					threadMain(void* param);


	public:
		//initialize DAC, DMA and GPIO
		static void 			init();
		//send i2c command
		static void 			send(unsigned char index, unsigned char data);
		//set volume through i2c
		static void 			setVolume(int db);
		//refill DMA
		static bool 			IRQdmaRefill();
		//Helper function used to start a DMA transfer from non-interrupt code
		static bool 			dmaRefill();
		//waits until a buffer is available for writing
		static unsigned short*  getWritableBuffer(miosix::Thread* t);
		//must be called when writing is finished
		static void				bufferFilled();


		//DMA end of transer interrupt handler
		static void				IRQdmaEndHandler();
};


#endif