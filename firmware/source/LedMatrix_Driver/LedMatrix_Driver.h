#ifndef LED_MATRIX_DRIVER__
#define LED_MATRIX_DRIVER__

#include <miosix.h>
#include "../config.h"
#include "miosix/kernel/scheduler/scheduler.h"

// TODO: Maybe I can speed up the GPIOs using ::speed (see gpio_impl.h)

#define LED_MATRIX_REFRESH_PERIOD	1	//ms

#define LED_MATRIX_ROWS				10
#define LED_MATRIX_COLUMNS			15

#define LED_SUBMATRIX_ROWS			5
#define LED_SUBMATRIX_COLUMNS		3

#define LED_MAX_CHARS				10
#define LED_HORIZONTAL_LAYERS		2
#define LED_VERTICAL_LAYERS 		5

typedef bool LedChar [LED_SUBMATRIX_ROWS][LED_SUBMATRIX_COLUMNS];	
typedef bool LedString [LED_MATRIX_ROWS][LED_MATRIX_COLUMNS];


//static class to manage led matrix
class LedMatrix_Driver
{
	private:
		//private contructor
		LedMatrix_Driver() {};

		static miosix::Thread*	refreshThread;
		static void 			configureTIM5();


	public:

		//initialize
		static void 			init();
		static void				fillVectors(); //Gpios are handled easier using them in Vectors
		static void				setGpiosMode(); //mode OUTPUT
		static void				turnOffAllLeds();
		static void				rowsOff();
		static void 			columnsOff();

		//thread safe writer methods
		static void 			setLed(unsigned short x, unsigned short y);
		static void				setLeds(unsigned short x[LED_MATRIX_ROWS], unsigned short y[LED_MATRIX_COLUMNS]);
		static void				setChar(LedChar ledChar, unsigned short ledHorizontalLayer, unsigned short ledVerticalLayer);
		static void				setString(std::string str);
		static void				writeLeds();
		static void				emptyBuffer();
		static void 			IRQTIM5Handler();
};
#endif