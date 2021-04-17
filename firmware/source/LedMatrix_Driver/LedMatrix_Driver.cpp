#include "LedMatrix_Driver.h"
#include "Leds_Gpios.h" // For Gpio namespace
#include "Leds_Chars.h" 

using namespace miosix;
using namespace ledsGpio;

miosix::Mutex 	LedMatrix_Driver::mutex;
miosix::Thread*	LedMatrix_Driver::refreshThread;

// miosix::GpioPin ROWS[LED_MATRIX_ROWS];
// miosix::GpioPin COLS[LED_MATRIX_COLUMNS];

std::vector<GpioPin> ROWS;
std::vector<GpioPin> COLS;

void LedMatrix_Driver::init()
{
	// Set OUTPUT mode on every led
	ROW1::mode(Mode::OUTPUT);
	ROW2::mode(Mode::OUTPUT);
	ROW3::mode(Mode::OUTPUT);
	ROW4::mode(Mode::OUTPUT);
	ROW5::mode(Mode::OUTPUT);
	ROW6::mode(Mode::OUTPUT);
	ROW7::mode(Mode::OUTPUT);
	ROW8::mode(Mode::OUTPUT);
	ROW9::mode(Mode::OUTPUT);
	ROW10::mode(Mode::OUTPUT);

	COL1::mode(Mode::OUTPUT);
	COL2::mode(Mode::OUTPUT);
	COL3::mode(Mode::OUTPUT);
	COL4::mode(Mode::OUTPUT);
	COL5::mode(Mode::OUTPUT);
	COL6::mode(Mode::OUTPUT);
	COL7::mode(Mode::OUTPUT);
	COL8::mode(Mode::OUTPUT);
	COL9::mode(Mode::OUTPUT);
	COL10::mode(Mode::OUTPUT);
	COL11::mode(Mode::OUTPUT);
	COL12::mode(Mode::OUTPUT);
	COL13::mode(Mode::OUTPUT);
	COL14::mode(Mode::OUTPUT);
	COL15::mode(Mode::OUTPUT);

	// Turn OFF all leds to avoid a disaster
	turnOffAllLeds();

	// Puts the GPIOs in 2 Vectors to facilitate their usage
	fillVectors();
	
	refreshThread = Thread::create(&LedMatrix_Driver::refreshThreadMain, STACK_MIN, 0);
}

void LedMatrix_Driver::fillVectors(){
	
	ROWS.push_back(ROW1::getPin());
	ROWS.push_back(ROW2::getPin());
	ROWS.push_back(ROW3::getPin());
	ROWS.push_back(ROW4::getPin());
	ROWS.push_back(ROW5::getPin());
	ROWS.push_back(ROW6::getPin());
	ROWS.push_back(ROW7::getPin());
	ROWS.push_back(ROW8::getPin());
	ROWS.push_back(ROW9::getPin());
	ROWS.push_back(ROW10::getPin());

	COLS.push_back(COL1::getPin());
	COLS.push_back(COL2::getPin());
	COLS.push_back(COL3::getPin());
	COLS.push_back(COL4::getPin());
	COLS.push_back(COL5::getPin());
	COLS.push_back(COL6::getPin());
	COLS.push_back(COL7::getPin());
	COLS.push_back(COL8::getPin());
	COLS.push_back(COL9::getPin());
	COLS.push_back(COL10::getPin());
	COLS.push_back(COL11::getPin());
	COLS.push_back(COL12::getPin());
	COLS.push_back(COL13::getPin());
	COLS.push_back(COL14::getPin());
	COLS.push_back(COL15::getPin());
}

void LedMatrix_Driver::turnOffAllLeds(){
	
	ROW1::high();
	ROW2::high();
	ROW3::high();
	ROW4::high();
	ROW5::high();
	ROW6::high();
	ROW7::high();
	ROW8::high();
	ROW9::high();
	ROW10::high();

	COL1::high();
	COL2::high();
	COL3::high();
	COL4::high();
	COL5::high();
	COL6::high();
	COL7::high();
	COL8::high();
	COL9::high();
	COL10::high();
	COL11::high();
	COL12::high();
	COL13::high();
	COL14::high();
	COL15::high();
}

void LedMatrix_Driver::setChar(const bool ledMatrix[LED_MATRIX_ROWS][LED_MATRIX_COLUMNS]){ 

	for (int i = 0; i < LED_MATRIX_ROWS; i++)
	{
		for (int j = 0; j < LED_MATRIX_COLUMNS; j++)
		{
			if (ledMatrix[i][j] == true)
			{
				ROWS[i].low();
				COLS[j].low();
				Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
			}
			if (ledMatrix[i][j] == false)
			{
				ROWS[i].high();
				COLS[j].high();
			}
		}
	}
}

void LedMatrix_Driver::refreshThreadMain(void *param)
{
	while(true)
	{
		//refresh next row
		setChar(A0);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A1);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A2);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A3);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A4);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A5);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A6);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A7);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A8);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

		setChar(A9);
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		turnOffAllLeds();

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

