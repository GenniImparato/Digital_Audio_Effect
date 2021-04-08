#ifndef AUDIO_EFFECT_SYNTH___H___
#define AUDIO_EFFECT_SYNTH___H___

#include "AudioEffect.h"

#define PI           		3.14159265358979323846 
#define SAMPLE_DURATION		(1.0/44100.0)


class Oscillator
{
	public:
		Oscillator(int wavetableSize=AUDIO_BUFFERS_SIZE, float frequency=440, float amplitude=0.5, int center=OUT_BUFFER_CENTER);
		virtual ~Oscillator();

		int				nextSample();
		void			setFrequency(float freq);
		void			setAmplitude(float ampl);
		void			setCenter(int center);
		void			setPhase(int phase);

	private:
		float				currIndex;
		float 				duration;

		float 				amplitude;
    	float 				frequency;
    	int 				center;
    	int					phase;

	protected:
		int					wavetableSize;
		int*  				wavetable;


};

class SawOscillator : public Oscillator
{
	public:
		SawOscillator(int wavetableSize=AUDIO_BUFFERS_SIZE, float frequency=440, float amplitude=0.5, int center=OUT_BUFFER_CENTER);
};

class SquareOscillator : public Oscillator
{
	public:
		SquareOscillator(int wavetableSize=AUDIO_BUFFERS_SIZE, float frequency=440, float amplitude=0.5, int center=OUT_BUFFER_CENTER);
};

class SineOscillator : public Oscillator
{
	public:
		SineOscillator(int wavetableSize=AUDIO_BUFFERS_SIZE, float frequency=440, float amplitude=0.5, int center=OUT_BUFFER_CENTER);
};


class Synthesizer	:public AudioEffect
{
	public:
		Synthesizer();
		~Synthesizer();

		//overriden methods
		void				preWrite();
		void				writeNextBuffer(unsigned short* wrBuff);
		void				postWrite();

	private:
		Oscillator*			osc1;
		Oscillator*			osc2;
		Oscillator*			osc3;

		float				freq = 100;
		bool 				rising = true;
		float				ampl = 0;
		bool 				rising2 = true;

};

#endif