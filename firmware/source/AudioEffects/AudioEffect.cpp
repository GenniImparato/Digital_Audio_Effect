#include "AudioEffect.h"
#include "../DAC_Driver/DAC_Driver.h"
#include "../ADC_Driver/ADC_Driver.h"
#include "../utils.h"

using namespace miosix;

AudioEffect::AudioEffect()
{
}

AudioEffect::~AudioEffect()
{
}


void AudioEffect::writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer)
{
	for(int i=0;i<AUDIO_BUFFERS_SIZE;i++)
		wrBuff[i] = rdBuffer[i];
}

void AudioEffect::postWrite()
{
}
