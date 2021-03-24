#ifndef 			CONFIGURATION_FILE_H
#define 			CONFIGURATION_FILE_H

#define 			AUDIO_BUFFERS_SIZE			256

typedef				miosix::BufferQueue<unsigned short,AUDIO_BUFFERS_SIZE>		AudioBufferQueue;

#endif