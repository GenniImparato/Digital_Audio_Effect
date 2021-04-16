#include "LedMatrix_Driver.h"

using namespace miosix;

typedef Gpio<GPIOD_BASE,11> row1;
typedef Gpio<GPIOD_BASE,9> row3;
typedef Gpio<GPIOD_BASE,7> col7;


miosix::Mutex 	LedMatrix_Driver::mutex;
miosix::Thread*	LedMatrix_Driver::refreshThread;


void LedMatrix_Driver::init()
{
	row1::mode(Mode::OUTPUT);
	row3::mode(Mode::OUTPUT);
	col7::mode(Mode::OUTPUT);

	refreshThread = Thread::create(&LedMatrix_Driver::refreshThreadMain, STACK_MIN, 0);
}

void LedMatrix_Driver::refreshThreadMain(void *param)
{
	while(true)
	{
		//refresh next row
		row1::high();
		col7::low();
		row3::low();

		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
	}
}

void LedMatrix_Driver::setLed(int x, int y, bool value)
{

}

void LedMatrix_Driver::writeChar(int x, int y, char c)
{

}

void LedMatrix_Driver::writeString(int y, char *stringPtr)
{

}