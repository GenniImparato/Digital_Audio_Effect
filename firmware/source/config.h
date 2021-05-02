#ifndef 			CONFIGURATION_FILE_H
#define 			CONFIGURATION_FILE_H

#define 			AUDIO_BUFFERS_SIZE			1024
#define				AUDIO_SAMPLING_FREQUENCY	44100.0

#define 			OUT_BUFFER_MAX 				65535
#define 			OUT_BUFFER_MIN				0
#define 			OUT_BUFFER_CENTER			((OUT_BUFFER_MAX-OUT_BUFFER_MIN)/2)

#define				POTS_COUNT					4


typedef				miosix::BufferQueue<unsigned short,AUDIO_BUFFERS_SIZE>		AudioBufferQueue;

#endif