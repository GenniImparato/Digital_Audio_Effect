#ifndef AUDIO_EFFECT_SYNTH___H___
#define AUDIO_EFFECT_SYNTH___H___

#include "AudioEffect.h"

#define PI           		3.14159265358979323846 
#define SAMPLE_DURATION		(1.0/44100.0)


class Oscillator
{
	private:
		float				currIndex;
		float 				duration;

	protected:
		int					wavetableSize;
		unsigned short*  	wavetable;

		virtual void 		makeWavetable();

	public:
		Oscillator(int wavetableSize);
		virtual ~Oscillator();

		unsigned short	nextSample();
		void			setFrequency(float freq);

		float amplitude = 2000;
    	float frequency = 172;
    	float phase 	= 0;
    	float time 		= 0;
   		float deltaTime = 1.0/AUDIO_SAMPLING_FREQUENCY;	

};

class TriangleOscillator : public Oscillator
{
	public:
		TriangleOscillator() : Oscillator(256) {};
};

class SineOscillator : public Oscillator
{
	private:
		void makeSamples();

	public:
		SineOscillator() : Oscillator(256) {};
};


class Synthesizer	:public AudioEffect
{
	public:
		Synthesizer();

		//overriden methods
		void				preWrite();
		void				writeNextBuffer(unsigned short* wrBuff);
		void				postWrite();

	private:
		void				makeSineSample(int sampleIndex);

		Oscillator*			osc1;

		float				freq = 100;
		bool 				rising = true;

};

#endif