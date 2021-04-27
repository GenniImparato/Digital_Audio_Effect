#ifndef LED_MATRIX_DRIVER__
#define LED_MATRIX_DRIVER__

#include <miosix.h>
#include "../config.h"
#include "miosix/kernel/scheduler/scheduler.h"

// TODO: Maybe I can speed up the GPIOs using ::speed (see gpio_impl.h)

#define LED_MATRIX_REFRESH_PERIOD	2	//ms

#define LED_MATRIX_ROWS				10
#define LED_MATRIX_COLUMNS			15
#define LED_CHARS					10

typedef bool LedChar [10][15];	


//static class to manage led matrix
class LedMatrix_Driver
{
	private:
		//private contructor
		LedMatrix_Driver() {};

		static void				refreshThreadMain(void *param);

		static miosix::Mutex 	mutex;
		static miosix::Thread*	refreshThread;
		static StringBufferQueue*	stringBuffer;
		static std::vector<bool [LED_MATRIX_ROWS][LED_MATRIX_COLUMNS]> ledChars;




	public:
		//initialize
		static void 			init();
		static void				fillVectors(); //Gpios are handled easier using them in Vectors
		static void				turnOffAllLeds();
		static void				rowsOff();
		static void 			columnsOff();

		//thread safe writer methods
		static void				writeChar(bool ledChar[LED_MATRIX_ROWS][LED_MATRIX_COLUMNS]);
		static void				writeString(StringBufferQueue *buffer);
};
#endif