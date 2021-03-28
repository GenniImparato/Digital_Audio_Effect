#ifndef 			CONFIGURATION_FILE_H
#define 			CONFIGURATION_FILE_H

#define 			AUDIO_BUFFERS_SIZE			128
#define				AUDIO_SAMPLING_FREQUENCY	44100.0

typedef				miosix::BufferQueue<unsigned short,AUDIO_BUFFERS_SIZE>		AudioBufferQueue;

#endif