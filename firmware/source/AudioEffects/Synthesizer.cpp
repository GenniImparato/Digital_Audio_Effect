#include "Synthesizer.h"
#include <math.h> 

Oscillator::Oscillator(int wavetableSize)
{
	this->wavetableSize = wavetableSize;
	this->currIndex = 0;
	this->duration = wavetableSize/44100.0;
	this->wavetable = new unsigned short[wavetableSize];

	makeWavetable();
}

Oscillator::~Oscillator()
{
	delete[] this->wavetable;
}

void Oscillator::makeWavetable()
{
	for(int i=0; i<wavetableSize; i++)
	{
		wavetable[i] = i*60000/wavetableSize;
	}
}

void Oscillator::setFrequency(float freq)
{
	this->frequency = freq;
}

unsigned short	Oscillator::nextSample()
{
	unsigned short ret = wavetable[(int)(currIndex)];
	currIndex += frequency*duration;
	if(currIndex >= wavetableSize)
		currIndex = 0;

	return ret;
}


void SineOscillator::makeSamples()
{
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
	osc1 = new Oscillator(256);
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
		freq += 0.5;
		if(freq>=1000)
			rising = false;
	}
	else
	{
		freq -= 0.5;
		if(freq<=50)
			rising = true;
	}
	osc1->setFrequency(freq);
}
