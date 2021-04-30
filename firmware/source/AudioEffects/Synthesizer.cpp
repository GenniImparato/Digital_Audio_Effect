#include "Synthesizer.h"
#include "../DAC_Driver/DAC_Driver.h"
#include <math.h> 

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

int	Oscillator::nextSample()
{
	int ret = amplitude*wavetable[(int)(currIndex)] + 0.5;
	currIndex += frequency*duration;
	if(currIndex >= wavetableSize)
		currIndex = 0;

	return ret;
}

float Oscillator::nextSampleF()
{
	float ret = amplitude*wavetable[(int)(currIndex)]/(float)wavetableSize + center;
	currIndex += frequency*duration;
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


Synthesizer::Synthesizer()	:AudioEffect()
{
	//344 hz square
	/*
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	{
		buff[i] = 60000;
	}
	for(int i=AUDIO_BUFFERS_SIZE; i<AUDIO_BUFFERS_SIZE*2; i++)
	{
		buff[i] = 0;
	}*/

	/*for(int i=0; i<AUDIO_BUFFERS_SIZE*2; i++)
	{
		buff[i] = osc1.getSample(i);
	}
	for(int i=AUDIO_BUFFERS_SIZE*2; i<AUDIO_BUFFERS_SIZE*3; i++)
	{
		buff[i] = osc1.getSample(2*(i-AUDIO_BUFFERS_SIZE*2));
	}*/
	osc1 = new SineOscillator(256, 500, 0.25, OUT_BUFFER_CENTER);
	osc2 = new SquareOscillator(256, 80, 0.25, OUT_BUFFER_CENTER);
	osc3 = new SawOscillator(256, 80, 0.25, OUT_BUFFER_CENTER);
}

Synthesizer::~Synthesizer()
{
	delete osc1;
	delete osc2;
	delete osc3;
}

void Synthesizer::preWrite()
{
}

void Synthesizer::writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	{
		wrBuff[i] = 0;
		if(ctrlValues[0] > 500)
			wrBuff[i] += osc1->nextSample()/3;
		if(ctrlValues[1] > 500)
			wrBuff[i] += osc2->nextSample()/3;
		if(ctrlValues[2] > 500)
			wrBuff[i] += osc3->nextSample()/3;
	}
}


void Synthesizer::postWrite()
{
	lfo1Incr = ctrlValues[3]/10000.0 + 0.001;
	if(lfo1Rising)
	{
		lfo1 += lfo1Incr;
		if(lfo1>=1.0)
		{
			lfo1 = 1.0;
			lfo1Rising = false;
		}
	}
	else
	{
		lfo1 -= lfo1Incr;
		if(lfo1<=0)
		{
			lfo1 = 0;
			lfo1Rising = true;
		}
	}

	osc1->setFrequency(ctrlValues[0]/3);
	osc2->setFrequency(ctrlValues[1]/3);
	osc2->setAmplitude(lfo1);
	osc3->setFrequency(ctrlValues[2]/3);
}
