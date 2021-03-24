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
		static unsigned short*		dmaZeroBuffer;
		static miosix::Thread*		waiting;

		//Helper function used to start a DMA transfer from non-interrupt code
		static void 			dmaRefill();


	public:
		//initialize DAC, DMA and GPIO
		static void 			init();
		//send i2c command
		static void 			send(unsigned char index, unsigned char data);
		//set volume through i2c
		static void 			setVolume(int db);
		//refill DMA
		static void 			IRQdmaRefill();
		//waits until a buffer is available for writing
		static unsigned short*  getWritableBuffer();
		//must be called when writing is finished
		static void				bufferFilled();

		//DAC main loop
		static void 			runThread();
		//DAC main thread loop
		static void				loop();


		//DMA end of transer interrupt handler
		static void				IRQdmaEndHandler();
};


#endif