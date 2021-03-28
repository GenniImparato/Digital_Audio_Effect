#include "Synthesizer.h"
#include <math.h> 

Oscillator::Oscillator(int wavetableSize, float frequency, float amplitude)
{
	this->wavetableSize = wavetableSize;
	this->currIndex = 0;
	this->duration = wavetableSize/AUDIO_SAMPLING_FREQUENCY;
	this->wavetable = new unsigned short[wavetableSize];

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

unsigned short	Oscillator::nextSample()
{
	unsigned short ret = amplitude * wavetable[(int)(currIndex)];
	currIndex += frequency*duration;
	if(currIndex >= wavetableSize)
		currIndex = 0;

	return ret;
}

SawOscillator::SawOscillator(int wavetableSize, float frequency, float amplitude) 
	:Oscillator(wavetableSize, frequency, amplitude)
{
	for(int i=0; i<wavetableSize; i++)
	{
		wavetable[i] = i*60000/wavetableSize;
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
	osc1 = new SawOscillator();
}

void Synthesizer::preWrite()
{
}

void Synthesizer::writeNextBuffer(unsigned short* wrBuff)
{
	for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
		wrBuff[i] = osc1->nextSample();
}


void Synthesizer::postWrite()
{
	if(rising)
	{
		freq += 15;
		if(freq>=800)
			rising = false;
	}
	else
	{
		freq -= 15;
		if(freq<=150)
			rising = true;
	}
	osc1->setFrequency(freq);

	if(rising2)
	{
		ampl += 0.04;
		if(ampl>=1)
		{
			rising2 = false;
			
		}
	}
	else
	{
		ampl -= 0.04;
		if(ampl<=0)
		{
			rising2 = true;
		}
		osc1->setAmplitude(ampl);
	}
}
