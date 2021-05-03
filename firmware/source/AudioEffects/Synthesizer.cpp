#include "Synthesizer.h"
#include "../DAC_Driver/DAC_Driver.h"
#include <math.h> 
#include <cstdlib> 
#include <ctime> 

using namespace std;

Oscillator::Oscillator(int wavetableSize, float frequency, float amplitude)
{
    this->wavetableSize = wavetableSize;
    this->currIndex = 0;
    this->phase = 0;
    this->duration = wavetableSize/AUDIO_SAMPLING_FREQUENCY;
    this->wavetable = new float[wavetableSize];

    setFrequency(frequency);
    setAmplitude(amplitude);

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

float Oscillator::nextSample()
{
    if(frequency < 40)
        return 0;

    float ret = amplitude*patternAmplitude*(wavetable[static_cast<int>(currIndex)]);

    currIndex += patternFrequency*frequency*duration;
    if(currIndex >= wavetableSize)
        currIndex = 0;

    return ret;
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

SawOscillator::SawOscillator(int wavetableSize, float frequency, float amplitude) 
    :Oscillator(wavetableSize, frequency, amplitude)
{
    for(int i=0; i<wavetableSize; i++)
    {
        wavetable[i] = 2.0*i/wavetableSize - 1.0;
    }
}

SquareOscillator::SquareOscillator(int wavetableSize, float frequency, float amplitude) 
    :Oscillator(wavetableSize, frequency, amplitude)
{
    for(int i=0; i<wavetableSize/2; i++)
    {
        wavetable[i] = 1;
    }
    for(int i=wavetableSize/2; i<wavetableSize; i++)
    {
        wavetable[i] = -1;
    }
}

SineOscillator::SineOscillator(int wavetableSize, float frequency, float amplitude) 
    :Oscillator(wavetableSize, frequency, amplitude)
{
    for(int i=0; i<wavetableSize; i++)
    {
        wavetable[i] = sin(2.0*PI*i/wavetableSize);
    }
}


Synthesizer::Synthesizer()  :AudioEffect()
{
    osc[0] = new SineOscillator(512, 500, 0.25);
    osc[1] = new SquareOscillator(512, 80, 0.25);
    osc[2] = new SawOscillator(512, 80, 0.25);

    for(int i=0; i<OSCILLATORS_COUNT; i++)
        holdControl[i] = false;

    srand((unsigned)time(0)); 

    controls[3].addChoiseName(string("FREQ"));
    controls[3].addChoiseName(string("AMPL"));
    controls[3].addChoiseName(string("MODE"));
    controls[3].addChoiseName(string("SPEED"));
}

Synthesizer::~Synthesizer()
{
    delete osc[0];
    delete osc[1];
    delete osc[2];
}


//inBuff not used, can be nullpntr
void Synthesizer::writeNextBuffer(float* inBuff, float* outBuff)
{
    for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
    {
        //outBuff[i] = osc[0]->nextSample()/3 + osc[1]->nextSample()/3 + osc[2]->nextSample()/3;
        outBuff[i] = osc[0]->nextSample()*0.3 + osc[1]->nextSample()*0.3 + osc[2]->nextSample()*0.3;
    }
}


void Synthesizer::postWrite()
{
    int activeParam = controls[3].getIntValue(0, 4);

    if(controls[3].isChangedInt(1))
    {
        for(int i=0; i<OSCILLATORS_COUNT; i++)
        {
            holdControl[i] = true;

            if(activeParam==0)
            {
                controls[i].setName(string("FREQ")+to_string(i+1));
                controls[i].setFloatDigits(0);
            }
            else if(activeParam==1)
            {
                controls[i].setName(string("AMPL")+to_string(i+1));
                controls[i].setFloatDigits(2);
            }
            else if(activeParam==2)
                controls[i].setName(string("MODE")+to_string(i+1));
            else if(activeParam==3)
                controls[i].setName(string("SPED")+to_string(i+1));
            
        }
            
    }

    for(int i=0; i<OSCILLATORS_COUNT; i++)
    {
        if(!holdControl[i])
        {
            if(activeParam==0)
                osc[i]->setFrequency(controls[i].getFloatValue(0, 4000));  
            else if(activeParam==1)
                osc[i]->setAmplitude(controls[i].getFloatValue(0, 0.3));   
            else if(activeParam==2)
                osc[i]->setPattern(controls[i].getIntValue(0, 4));
            else if(activeParam==3)
                osc[i]->setPatternSpeed(controls[i].getIntValue(0, PATTERN_MAX_SPEED-5));
        }
        else if(controls[i].isPotMoved(10))
            holdControl[i] = false;

        osc[i]->update();
    }
}
