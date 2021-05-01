#include "Synthesizer.h"
#include "../DAC_Driver/DAC_Driver.h"
#include <math.h> 
#include <cstdlib> 
#include <ctime> 

Oscillator::Oscillator(int wavetableSize, float frequency, float amplitude, int center)
{
    this->wavetableSize = wavetableSize;
    this->currIndex = 0;
    this->phase = 0;
    this->duration = wavetableSize/AUDIO_SAMPLING_FREQUENCY;
    this->wavetable = new int[wavetableSize];

    setFrequency(frequency);
    setAmplitude(amplitude);
    setCenter(center);

    for(int i=0; i<wavetableSize; i++)
    {
        wavetable[i] = 0;
    }
}

Oscillator::~Oscillator()
{
    delete[] this->wavetable;
}

void Oscillator::setFrequency(float freq)
{
    this->frequency = freq;
}

void Oscillator::setAmplitude(float ampl)
{
    this->amplitude = ampl;
}

void Oscillator::setCenter(int center)
{
    this->center = center;
}

void Oscillator::setPhase(int phase)
{
    this->currIndex+= phase;
}

void Oscillator::setPattern(int pattern)
{
    this->pattern = pattern;
}

void Oscillator::setPatternSpeed(int speed)
{
    this->patternSpeed = PATTERN_MAX_SPEED - speed;
}

int Oscillator::nextSample()
{
    if(frequency < 40)
        return 0;

    float ret = amplitude*patternAmplitude*(float)(wavetable[(int)(currIndex)]);

    currIndex += patternFrequency*frequency*duration;
    if(currIndex >= wavetableSize)
        currIndex = 0;

    return (int)ret;
}

void Oscillator::update()
{
    patternIndex1 ++;
    if(patternIndex1 >= patternSpeed)
    {
        patternIndex1 = 0;

        patternIndex2++;
        patternIndex2Updated=true;
        if(patternIndex2 >= 10)
            patternIndex2=0;
    }

    //NORMAL
    if(pattern == 0)
    {
        patternFrequency = 1;
        patternAmplitude = 1;
    }
    //ARPEGGIO
    else if(pattern == 1)
    {
        patternFrequency = 1 + 0.1*patternIndex2;
        patternAmplitude = 1;
    }
    //RANDOM
    else if(pattern == 2)
    {
        if(patternIndex2Updated)
        {
            patternFrequency = (float)(rand()%1000)/1000;
            patternAmplitude = 1;
        }
    }
    //RANDOM
    else if(pattern == 3)
    {
        if(patternIndex2Updated == 0)
        {
            if(patternIndex2 == 0)
            {
                patternFrequency = 1;
                patternAmplitude = 1;
            }
            else if(patternIndex2 == 1)
            {
                patternAmplitude = 0;
            }
            else if(patternIndex2 == 2)
            {
                patternFrequency = 2;
                patternAmplitude = 1;
            }
            else if(patternIndex2 == 3)
            {
                patternAmplitude = 0;
            }
            else if(patternIndex2 == 4)
                patternIndex2 = 0;
        }
        else
        {
            patternAmplitude = 0.0;
        }
    }

    patternIndex2Updated=false;
}

float Oscillator::nextSampleF()
{
    float ret = amplitude*wavetable[(int)(currIndex)]/(float)wavetableSize + center;
    currIndex += patternFrequency*duration;
    if(currIndex >= wavetableSize)
        currIndex = 0;

    return ret;
}

SawOscillator::SawOscillator(int wavetableSize, float frequency, float amplitude, int center) 
    :Oscillator(wavetableSize, frequency, amplitude, center)
{
    for(int i=0; i<wavetableSize; i++)
    {
        wavetable[i] = center + i*60000/wavetableSize - 30000;
    }
}

SquareOscillator::SquareOscillator(int wavetableSize, float frequency, float amplitude, int center) 
    :Oscillator(wavetableSize, frequency, amplitude, center)
{
    for(int i=0; i<wavetableSize/2; i++)
    {
        wavetable[i] = center + 30000;
    }
    for(int i=wavetableSize/2; i<wavetableSize; i++)
    {
        wavetable[i] = center -30000;
    }
}

SineOscillator::SineOscillator(int wavetableSize, float frequency, float amplitude, int center) 
    :Oscillator(wavetableSize, frequency, amplitude, center)
{
    for(int i=0; i<wavetableSize; i++)
    {
        wavetable[i] = center + 30000*sin(2*PI*i/wavetableSize);
    }
}


Synthesizer::Synthesizer()  :AudioEffect()
{
    controls[0] = new EffectControl();
    controls[1] = new EffectControl();
    controls[2] = new EffectControl();
    controls[3] = new EffectControl();


    osc[0] = new SineOscillator(256, 500, 0.25, OUT_BUFFER_CENTER);
    osc[1] = new SquareOscillator(256, 80, 0.25, OUT_BUFFER_CENTER);
    osc[2] = new SawOscillator(256, 80, 0.25, OUT_BUFFER_CENTER);

    for(int i=0; i<OSCILLATORS_COUNT; i++)
        holdControl[i] = false;

    srand((unsigned)time(0)); 
}

Synthesizer::~Synthesizer()
{
    delete osc[0];
    delete osc[1];
    delete osc[2];
}


void Synthesizer::writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer=nullptr)
{
    for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
    {
        wrBuff[i] = osc[0]->nextSample()/3 + osc[1]->nextSample()/3 + osc[2]->nextSample()/3;
    }
}


void Synthesizer::postWrite()
{
    int activeParam = controls[3]->getIntValue(0, 4);

    if(controls[3]->isChangedInt(1))
    {
        for(int i=0; i<OSCILLATORS_COUNT; i++)
            holdControl[i] = true;
    }

    for(int i=0; i<OSCILLATORS_COUNT; i++)
    {
        if(!holdControl[i])
        {
            if(activeParam==0)
                osc[i]->setFrequency(controls[i]->getFloatValue(0, 4000));  
            else if(activeParam==1)
                osc[i]->setAmplitude(controls[i]->getFloatValue(0, 0.3));   
            else if(activeParam==2)
                osc[i]->setPattern(controls[i]->getIntValue(0, 4));
            else if(activeParam==3)
                osc[i]->setPatternSpeed(controls[i]->getIntValue(0, PATTERN_MAX_SPEED-5));
        }
        else if(controls[i]->isPotMoved(60))
            holdControl[i] = false;

        osc[i]->update();
    }
}
