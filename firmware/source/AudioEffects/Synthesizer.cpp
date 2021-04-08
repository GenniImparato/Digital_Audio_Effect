#include "Synthesizer.h"
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
	int ret = amplitude*wavetable[(int)(currIndex)] + center;
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
		wavetable[i] = i*60000/wavetableSize - 30000;
	}
}

SquareOscillator::SquareOscillator(int wavetableSize, float frequency, float amplitude, int center) 
	:Oscillator(wavetableSize, frequency, amplitude, center)
{
	for(int i=0; i<wavetableSize/2; i++)
	{
		wavetable[i] = 30000;
	}
	for(int i=wavetableSize/2; i<wavetableSize; i++)
	{
		wavetable[i] = -30000;
	}
}

SineOscillator::SineOscillator(int wavetableSize, float frequency, float amplitude, int center) 
	:Oscillator(wavetableSize, frequency, amplitude, center)
{
	for(int i=0; i<wavetableSize; i++)
	{
		wavetable[i] = 30000*sin(2*PI*i/wavetableSize);
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
	osc1 = new SineOscillator(256, 100, 0.25, 0);
	osc2 = new SineOscillator(512, 80, 0.25, 0);
	osc3 = new SquareOscillator(512, 80, 0.02, 0);
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

void Synthesizer::writeNextBuffer(unsigned short* wrBuff)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
		wrBuff[i] = OUT_BUFFER_CENTER + osc1->nextSample() + osc2->nextSample() + osc3->nextSample();
}


void Synthesizer::postWrite()
{
	if(rising)
	{
		freq += 0.5;
		if(freq>=150)
			rising = false;
	}
	else
	{
		freq -= 0.5;
		if(freq<=-0)
			rising = true;
	}
	osc1->setFrequency(440 + freq);
	osc2->setFrequency(20 + freq*1.29);
	osc3->setFrequency(912 - freq*3.91);
}
