#include "LedMatrix_Driver.h"
#include "Leds_Gpios.h" // For Gpio namespace
#include "Leds_Chars.h" 
// #include <cstdint>
#include <cstring> // To use memcpy
#include <cctype> // To use toupper()

using namespace miosix;
using namespace ledsGpio;

miosix::Mutex 	LedMatrix_Driver::mutex;
miosix::Thread*	LedMatrix_Driver::refreshThread;
StringBufferQueue* LedMatrix_Driver::stringBuffer;


std::vector<GpioPin> ROWS;
std::vector<GpioPin> COLS;


void addMatrices(LedString result, LedString ledChar){
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

void LedMatrix_Driver::setChar(LedString ledString ,LedChar ledChar, int ledMatrixLayer, int ledMatrixColumn){

	for (int i = LED_SUBMATRIX_ROWS*ledMatrixLayer; i < LED_MATRIX_ROWS; i++){
		for (int j = LED_SUBMATRIX_COLUMNS*ledMatrixColumn; j < LED_MATRIX_COLUMNS; j++){
			if (ledString[i][j] == 0 && (i - LED_SUBMATRIX_ROWS*ledMatrixLayer) < LED_SUBMATRIX_ROWS && (j - LED_SUBMATRIX_COLUMNS*ledMatrixColumn) < LED_SUBMATRIX_COLUMNS)		
				ledString[i][j] += ledChar[i - LED_SUBMATRIX_ROWS*ledMatrixLayer][j - LED_SUBMATRIX_COLUMNS*ledMatrixColumn];
		}
	}
}

//TODO: porre vincoli su lunghezza stringa
//TODO: migliorare il codice
void LedMatrix_Driver::setString(LedString *bufStr , std::string str){

	unsigned int layerCount = 0;
	unsigned int columnCount = 0;
	unsigned int letterCount = 0;

	for (unsigned int i = 0; i < str.length(); i++){

		if (toupper(str[i]) == 'A'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], A, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'B'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], B, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'C'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], C, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'D'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], D, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'E'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], E, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'F'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], F, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'G'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], G, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'H'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], H, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'I'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], I, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'J'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], J, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'K'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], K, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'L'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], L, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'M'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], M, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'N'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], N, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'O'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], O, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'P'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], P, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'Q'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], Q, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'R'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], R, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'S'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], S, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'T'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], T, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'U'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], U, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'V'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], V, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'W'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], W, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'X'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], X, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'Y'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], Y, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == 'Z'){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], Z, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
		if (toupper(str[i]) == ' '){
			if(letterCount == 5 && layerCount <= 1){
				layerCount++;
				columnCount = 0;
			}
			setChar(bufStr[0], SPACE, layerCount, columnCount);
			if (columnCount < 4)
			{
				columnCount++;
			}
			letterCount++;
		}
	}
}

void LedMatrix_Driver::writeLeds(LedString ledChar){ 
	for (int i = 0; i < LED_MATRIX_ROWS; i++){
		ROWS[i].low();
		for (int j = 0; j < LED_MATRIX_COLUMNS; j++){
			if (ledChar[i][j] == true){
				COLS[j].low();
			}
			if (ledChar[i][j] == false){
				COLS[j].high();
			}
		}
		Thread::sleep(LED_MATRIX_REFRESH_PERIOD);
		ROWS[i].high();
		columnsOff();
	}
}

void LedMatrix_Driver::refreshThreadMain(void *param){
	while(true)
	{
		//refresh next row

		/* 	ledMatrixLayer: can have values 0-1 
				when 0, it targets rows 0-4
				when 1, it targets rows 5-9
			ledMatrixColumn: can have value 0-4
				when 0, it targets columns 0-2
				when 1, it targets columns 3-5
				...
				when 4, it targets columns 12-14
		*/
		LedString *bufStr {};
		stringBuffer->tryGetWritableBuffer(bufStr);
		LedString zeroMatrix {0};
		std::memcpy(bufStr[0], zeroMatrix, sizeof(LedString));

		setString(bufStr, "bellagenni");
		writeLeds(bufStr[0]);

	}
}




