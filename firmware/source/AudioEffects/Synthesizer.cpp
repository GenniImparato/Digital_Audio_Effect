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
	this->basefrequency = freq;
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

int	Oscillator::nextSample()
{
	if(basefrequency < 40)
		return 0;

	float ret = amplitude*patternAmplitude*(float)(wavetable[(int)(currIndex)]);

	currIndex += patternFrequency*duration;
	if(currIndex >= wavetableSize)
		currIndex = 0;

	return (int)ret;
}

void Oscillator::update()
{
	patternIndex1 ++;
	if(patternIndex1 >= 10)
	{
		patternIndex1 = 0;

		patternIndex2++;
		patternIndex2Updated=true;
		if(patternIndex2 >= 10)
			patternIndex2=0;
	}

	//NORMAL
	if(pattern == 0)
		patternFrequency = basefrequency;
	//ARPEGGIO
	else if(pattern == 1)
		patternFrequency = basefrequency+200*patternIndex2;
	//RANDOM
	else if(pattern == 2)
	{
		if(patternIndex2Updated)
		{
			patternIndex2Updated = false;
			patternFrequency = 3*(rand()%1000)+20;
		}
	}
	//RANDOM
	else if(pattern == 3)
	{
		if(patternIndex2%2 == 0)
		{
			patternAmplitude = 1.0;
		}
		else
		{
			patternAmplitude = 0.0;
		}
	}
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


Synthesizer::Synthesizer()	:AudioEffect()
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

void Synthesizer::preWrite()
{
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
	/*lfo1Incr = ctrlValues[3]/10000.0 + 0.001;
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
	}*/

	int activeParam = controls[3]->getIntValue(0, 3);

	if(controls[3]->isChangedInt(1))
	{
		for(int i=0; i<OSCILLATORS_COUNT; i++)
			holdControl[i] = true;
	}

	for(int i=0; i<OSCILLATORS_COUNT; i++)
	{
		if(activeParam==0)
		{
			float param = controls[i]->getFloatValue(0, 4000);
			if(!holdControl[i])
				osc[i]->setFrequency(param);	
			else if(controls[i]->isPotMoved(60))
				holdControl[i] = false;
		}
		else if(activeParam==1)
		{
			float param = controls[i]->getFloatValue(0, 0.3);
			if(!holdControl[i])
				osc[i]->setAmplitude(param);	
			else if(controls[i]->isPotMoved(60))
				holdControl[i] = false;
		}
		else if(activeParam==2)
		{
			
			int param = controls[i]->getIntValue(0, 4);
			if(!holdControl[i])
				osc[i]->setPattern(param);
			else if(controls[i]->isPotMoved(60))
				holdControl[i] = false;
		}

		osc[i]->update();
	}

	

	/*if(!controlHold[0])
	{

	}*/
	


	/*else if(activeOsc == 1)
	{
		osc2->setFrequency(freq);
		osc2->setAmplitude(ampl);
		osc2->setPattern(pattern);
	}
	else if(activeOsc == 2)
	{
		osc3->setFrequency(freq);
		osc3->setAmplitude(ampl);
		osc3->setPattern(pattern);
	}*/
	/*osc2->setFrequency(controls[1].getValue());
	osc2->setAmplitude(lfo1);
	osc3->setFrequency(controls[2].getValue());*/

}
