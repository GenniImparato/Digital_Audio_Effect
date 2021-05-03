#include "LedMatrix_Driver.h"
#include "Leds_Gpios.h" // For Gpio namespace
#include "Leds_Chars.h"
#include <cstring> // To use memcpy
#include <cctype> // To use toupper()
#include <mutex>

using namespace miosix;

miosix::Mutex 	LedMatrix_Driver::mutex;
miosix::ConditionVariable LedMatrix_Driver::cv;
miosix::Thread*	LedMatrix_Driver::refreshThread;
LedString bufStr {}; //Buffer
bool isSetting = false;

std::vector<GpioPin> ROWS;
std::vector<GpioPin> COLS;

void checkHorizontalLayer(unsigned short &letterCount, unsigned short &horizontalLayerCount, unsigned short &verticalLayerCount){
	if(letterCount == LED_MAX_CHARS/2 && horizontalLayerCount <= LED_HORIZONTAL_LAYERS-1){
		horizontalLayerCount++;
		verticalLayerCount = 0;
	}
}

void checkVerticalLayer(unsigned short &verticalLayerCount){
	if (verticalLayerCount < LED_VERTICAL_LAYERS - 1)
	{
		verticalLayerCount++;
	}
}

void LedMatrix_Driver::init()
{
	// Puts the GPIOs in 2 Vectors to facilitate their usage
	fillVectors();

	// Set OUTPUT mode on every led
	setGpiosMode();

	// Turn OFF all leds since setting their mode to OUTPUT turns them on
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

void LedMatrix_Driver::setGpiosMode(){
	// Set OUTPUT mode on every led
	for (int i = 0; i < LED_MATRIX_ROWS; i++)
	{
		ROWS[i].mode(Mode::OUTPUT);
	}
	for (int i = 0; i < LED_MATRIX_COLUMNS; i++)
	{
		COLS[i].mode(Mode::OUTPUT);
	}

}

void LedMatrix_Driver::turnOffAllLeds(){
	rowsOff();
	columnsOff();
}

void LedMatrix_Driver::rowsOff(){
	for (unsigned int i = 0; i < ROWS.size(); i++){
		ROWS[i].high();
	}
}

void LedMatrix_Driver::columnsOff(){
	for (unsigned int i = 0; i < COLS.size(); i++){
		COLS[i].high();
	}	
}

/* 	ledHorizontalLayer: can have values 0-1 
		when 0, it targets rows 0-4
		when 1, it targets rows 5-9
	ledVerticalLayer: can have value 0-4
		when 0, it targets columns 0-2
		when 1, it targets columns 3-5
		...
		when 4, it targets columns 12-14
*/
void LedMatrix_Driver::setChar(LedChar ledChar, unsigned short ledHorizontalLayer, unsigned short ledVerticalLayer){
	for (int i = LED_SUBMATRIX_ROWS*ledHorizontalLayer; i < LED_MATRIX_ROWS; i++){
		for (int j = LED_SUBMATRIX_COLUMNS*ledVerticalLayer; j < LED_MATRIX_COLUMNS; j++){
			if (bufStr[i][j] == 0 && (i - LED_SUBMATRIX_ROWS*ledHorizontalLayer) < LED_SUBMATRIX_ROWS && (j - LED_SUBMATRIX_COLUMNS*ledVerticalLayer) < LED_SUBMATRIX_COLUMNS)		
				bufStr[i][j] += ledChar[i - LED_SUBMATRIX_ROWS*ledHorizontalLayer][j - LED_SUBMATRIX_COLUMNS*ledVerticalLayer];
		}
	}
}

void LedMatrix_Driver::setString(std::string str){

	unsigned short letterCount = 0;
	unsigned short horizontalLayerCount = 0;
	unsigned short verticalLayerCount = 0;
	// isSetting = true;

	emptyBuffer();

	// miosix::Lock<miosix::Mutex> lock(mutex);

	if (str.length() > LED_MAX_CHARS){
		setString("Too  Long");
		return;
	}

	for (unsigned int i = 0; i < str.length(); i++){
		checkHorizontalLayer(letterCount, horizontalLayerCount, verticalLayerCount);
		switch (toupper(str[i])){
		case 'A':
			setChar(A, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'B':
			setChar(B, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'C':
			setChar(C, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'D':
			setChar(D, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'E':
			setChar(E, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'F':
			setChar(F, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'G':
			setChar(G, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'H':
			setChar(H, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'I':
			setChar(I, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'J':
			setChar(J, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'K':
			setChar(K, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'L':
			setChar(L, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'M':
			setChar(M, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'N':
			setChar(N, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'O':
			setChar(O, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'P':
			setChar(P, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'Q':
			setChar(Q, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'R':
			setChar(R, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'S':
			setChar(S, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'T':
			setChar(T, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'U':
			setChar(U, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'V':
			setChar(V, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'W':
			setChar(W, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'X':
			setChar(X, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'Y':
			setChar(Y, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case 'Z':
			setChar(Z, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '0':
			setChar(ZERO, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '1':
			setChar(ONE, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '2':
			setChar(TWO, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '3':
			setChar(THREE, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '4':
			setChar(FOUR, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '5':
			setChar(FIVE, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '6':
			setChar(SIX, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '7':
			setChar(SEVEN, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '8':
			setChar(EIGHT, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '9':
			setChar(NINE, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		case '.':
			setChar(POINT, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		default:
			setChar(SPACE, horizontalLayerCount, verticalLayerCount);
			letterCount++;
			break;
		}	
		checkVerticalLayer(verticalLayerCount);
	}

	// isSetting = false;
	// cv.signal();
}

void LedMatrix_Driver::writeLeds(){ 
		
	for (int i = 0; i < LED_MATRIX_ROWS; i++){
		// miosix::Lock<miosix::Mutex> lock(mutex);
		// while (isSetting)
		// 	cv.wait(lock);

		ROWS[i].low();
		for (int j = 0; j < LED_MATRIX_COLUMNS; j++){
			if (bufStr[i][j] == true){
				COLS[j].low();
			}
			if (bufStr[i][j] == false){
				COLS[j].high();
			}
		}

		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		ROWS[i].high();
		columnsOff();
	}
}

void LedMatrix_Driver::emptyBuffer(){
	for (unsigned int i = 0; i < LED_MATRIX_ROWS; i++){
		// miosix::Lock<miosix::Mutex> lock(mutex);
		for (unsigned int j = 0; j < LED_MATRIX_COLUMNS; j++){
			bufStr[i][j] = 0;
		}
	}
}

void LedMatrix_Driver::refreshThreadMain(void *param){
	while(true)
	{
		//refresh next row
		writeLeds();

	}
}




