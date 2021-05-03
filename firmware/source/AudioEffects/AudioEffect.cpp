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

//copies input to output
void AudioEffect::writeNextBuffer(float* inBuff, float* outBuff)
{
    for(int i=0;i<AUDIO_BUFFERS_SIZE;i++)
        outBuff[i] = inBuff[i];
}

void AudioEffect::setControlFromPot(unsigned int control, unsigned int value)
{
    if(control>=POTS_COUNT)
        return;

    controls[control].setValueFromPot(value);
}

EffectControl* AudioEffect::getControl(int control)
{
    if(control>=POTS_COUNT)
        return nullptr;

    return &controls[control];
}
