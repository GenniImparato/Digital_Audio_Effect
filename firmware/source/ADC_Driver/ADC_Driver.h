#ifndef ADC_DRIVER___H__
#define ADC_DRIVER___H__

#include <miosix.h>
#include "../config.h"
#include "miosix/kernel/scheduler/scheduler.h"
#include "Filter.h"

#define	FILTER_SAMLPLES_COUNT		5

//static class to manage ADC
class ADC_Driver
{
	private:
		//private contructor, ADC_Driver has only static members
		ADC_Driver() {};

		//potentiometers/channels map
		static const unsigned short potChannels[POTS_COUNT];

		static AudioBufferQueue*   	dmaBuffer;
		static miosix::Thread*		waitingThread;
		static miosix::Thread*		restartThread;
		static bool 				dmaRefillWaiting;

		static unsigned short buffer[AUDIO_BUFFERS_SIZE];

		static void 				    configureTIM2();
		static void						configureADC2();
		static void  					adc2Restart();

	public:

		//initialize ADC
		static void		 				init();
		
		//performs a single conversion (uses ADC1)
		static unsigned int 			singleConversion(unsigned short channel);

		//performs many conversions and filter (uses ADC1)
		static unsigned int 			filterConversions(unsigned short channel);

		//performs a single conversion given pot id (uses ADC1)
		static unsigned int 			filterConversionsPot(unsigned short pot);

		//suspends readerThread until a buffer is available for reading (uses ADC2)
		static const unsigned short * 	getReadableBuffer(miosix::Thread* readerThread);

		//Configure the DMA to do another transfer
		static bool 					IRQdmaRestart();

		//Configure the DMA to do another transfer from non-interrupt code
		static bool 					dmaRestart();

		static void 					threadMain(void *param);

		

		static void 					bufferEmptied();


		static void 					IRQdmaEndHandler();
		static void 					IRQTIM2Handler(void);
};

#endif