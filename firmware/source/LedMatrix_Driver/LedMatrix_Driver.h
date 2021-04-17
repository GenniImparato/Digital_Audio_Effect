#ifndef LED_MATRIX_DRIVER__
#define LED_MATRIX_DRIVER__

#include <miosix.h>
#include "../config.h"
#include "miosix/kernel/scheduler/scheduler.h"


#define LED_MATRIX_REFRESH_PERIOD	100	//ms

#define LED_MATRIX_ROWS				10
#define LED_MATRIX_COLUMNS			15

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
		static void				fillVectors(); //Gpios are handled easier using them in Vectors


		//thread safe writer methods
		static void				turnOffAllLeds();
		static void				setChar(const bool ledMatrix[LED_MATRIX_ROWS][LED_MATRIX_COLUMNS]);
		static void				setLed(int x, int y, bool value);
		static void 			writeChar(int x, int y, char c);
		static void 			writeString(int y, char *stringPtr);


};


#endif