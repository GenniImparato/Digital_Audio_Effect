#ifndef AUDIO_EFFECT_SYNTH___H___
#define AUDIO_EFFECT_SYNTH___H___

#include "AudioEffect.h"

#define PI           		3.14159265358979323846 
#define SAMPLE_DURATION		(1.0/44100.0)


class Oscillator
{
	public:
		Oscillator(int wavetableSize=AUDIO_BUFFERS_SIZE, float frequency=440, float amplitude=0.5);
		virtual ~Oscillator();

		unsigned short	nextSample();
		void			setFrequency(float freq);
		void			setAmplitude(float ampl);

	private:
		float				currIndex;
		float 				duration;

		float 				amplitude;
    	float 				frequency;

	protected:
		int					wavetableSize;
		unsigned short*  	wavetable;


};

class SawOscillator : public Oscillator
{
	public:
		SawOscillator(int wavetableSize=AUDIO_BUFFERS_SIZE, float frequency=440, float amplitude=0.5);
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
		Oscillator*			osc1;
		Oscillator*			osc2;

		float				freq = 100;
		bool 				rising = true;
		float				ampl = 0;
		bool 				rising2 = true;

};

#endif