#include "LedMatrix_Driver.h"
#include "Leds_Gpios.h" // For Gpio namespace
#include "Leds_Chars.h" 
// #include <cstdint>
#include <cstring> // To use memcpy

using namespace miosix;
using namespace ledsGpio;

miosix::Mutex 	LedMatrix_Driver::mutex;
miosix::Thread*	LedMatrix_Driver::refreshThread;
StringBufferQueue* LedMatrix_Driver::stringBuffer;

std::vector<GpioPin> ROWS;
std::vector<GpioPin> COLS;


void addMatrices(LedChar result, LedChar ledChar){
	for (int i = 0; i < LED_MATRIX_ROWS; i++){
		for (int j = 0; j < LED_MATRIX_COLUMNS; j++){
			if (result[i][j] == 0)		
				result[i][j] += ledChar[i][j];
		}
	}

} 

void LedMatrix_Driver::init()
{
	// Puts the GPIOs in 2 Vectors to facilitate their usage
	fillVectors();

	// Buffer creation
	stringBuffer = new StringBufferQueue();

	// Set OUTPUT mode on every led
	for (int i = 0; i < LED_MATRIX_ROWS; i++)
	{
		ROWS[i].mode(Mode::OUTPUT);
	}
	for (int i = 0; i < LED_MATRIX_COLUMNS; i++)
	{
		COLS[i].mode(Mode::OUTPUT);
	}

	// Turn OFF all leds to avoid a disaster
	turnOffAllLeds();

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
	rowsOff();
	columnsOff();
}

void LedMatrix_Driver::rowsOff() {
	for (unsigned int i = 0; i < ROWS.size(); i++)
	{
		ROWS[i].high();
	}
	
}

void LedMatrix_Driver::columnsOff() {
	for (unsigned int i = 0; i < COLS.size(); i++)
	{
		COLS[i].high();
	}
	
}

void LedMatrix_Driver::writeChar(LedChar ledChar){ 
	for (int i = 0; i < LED_MATRIX_ROWS; i++){
		ROWS[i].low();
		for (int j = 0; j < LED_MATRIX_COLUMNS; j++){
			if (ledChar[i][j] == true){
				COLS[j].low();
				Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
			}
			if (ledChar[i][j] == false){
				COLS[j].high();
			}
		}
		ROWS[i].high();
		columnsOff();
	}
}

void LedMatrix_Driver::writeString(StringBufferQueue *buffer){

	LedChar ledString {0}; // Resulting string, which is just a single boolean matrix
	//
	LedChar *str;

	buffer->tryGetWritableBuffer(str);
	for (int i = 0; i < LED_CHARS; i++){
		std::memcpy(str[i], zeroMatrix, sizeof(bool[10][15]));
	}
	
	std::memcpy(str[0], A0, sizeof(bool[10][15]));
	std::memcpy(str[1], C1, sizeof(bool[10][15]));

	for (int i = 0; i < LED_CHARS; i++)
	{
		addMatrices(ledString, str[i]);
	}

	writeChar(ledString);
	
	// Notifies the other threads that the buffer is full and ready to be read
	// buffer->bufferFilled(LED_CHARS);
}

void LedMatrix_Driver::refreshThreadMain(void *param)
{
	while(true)
	{
		//refresh next row
		writeString(stringBuffer);
	}
}
