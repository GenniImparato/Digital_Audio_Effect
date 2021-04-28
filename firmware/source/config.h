#ifndef 			CONFIGURATION_FILE_H
#define 			CONFIGURATION_FILE_H

#define 			AUDIO_BUFFERS_SIZE			1024
#define				AUDIO_SAMPLING_FREQUENCY	44100.0

#define             STRING_BUFFERS_SIZE         10
#define             LED_MATRIX_ROWS             10
#define             LED_MATRIX_COLUMNS          15



typedef				miosix::BufferQueue<unsigned short,AUDIO_BUFFERS_SIZE>		AudioBufferQueue;
typedef             miosix::BufferQueue<bool[LED_MATRIX_ROWS][LED_MATRIX_COLUMNS],STRING_BUFFERS_SIZE> StringBufferQueue;

#endif