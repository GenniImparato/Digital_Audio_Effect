#ifndef LED_MATRIX_DRIVER__
#define LED_MATRIX_DRIVER__

#include <miosix.h>
#include "../config.h"
#include "miosix/kernel/scheduler/scheduler.h"


#define LED_MATRIX_REFRESH_PERIOD	50		//ms

#define LED_MATRIX_ROWS				15
#define LED_MATRIX_COLUMNS			10

//static class to manage led matrix
class LedMatrix_Driver
{
	private:
		//private contructor
		LedMatrix_Driver() {};

		static void				refreshThreadMain(void *param);

		static miosix::Mutex 	mutex;
		static miosix::Thread*	refreshThread;

	public:
		//initialize
		static void 			init();

		//thread safe writer methods
		static void				setLed(int x, int y, bool value);
		static void 			writeChar(int x, int y, char c);
		static void 			writeString(int y, char *stringPtr);

		
};


#endif