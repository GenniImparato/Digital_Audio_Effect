#ifndef AUDIO_EFFECT_SYNTH___H___
#define AUDIO_EFFECT_SYNTH___H___

#include "AudioEffect.h"


#define PI                  3.14159265358979323846 
#define SAMPLE_DURATION     (1.0/44100.0)
#define OSCILLATORS_COUNT   3

#define PATTERN_MAX_SPEED   50


class Oscillator
{
    public:
        Oscillator(int wavetableSize=AUDIO_BUFFERS_SIZE, float frequency=440, float amplitude=0.5, int center=OUT_BUFFER_CENTER);
        virtual ~Oscillator();

        int             nextSample();
        float           nextSampleF();
        void            setFrequency(float freq);
        void            setAmplitude(float ampl);
        void            setCenter(int center);
        void            setPhase(int phase);
        void            setPattern(int pattern);
        void            setPatternSpeed(int speed);
        void            update();

    private:
        float           currIndex;
        float           duration;
        float           amplitude;
        float           patternAmplitude = 1;
        float           frequency;
        float           patternFrequency = 0;
        int             center;
        int             phase;
        int             pattern = 0;
        int             patternIndex1 = 0;
        int             patternIndex2 = 0;
        bool            patternIndex2Updated = false;
        int             patternSpeed = 10;

    protected:
        int             wavetableSize;
        int*            wavetable;

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


class Synthesizer   :public AudioEffect
{
    public:
        Synthesizer();
        ~Synthesizer();

        //overriden methods
        void                writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer);
        void                postWrite();

    private:
        Oscillator*         osc[OSCILLATORS_COUNT];

        float               lfo1 = 100;
        bool                lfo1Rising = true;
        float               lfo1Incr = 0.01;
        float               ampl = 0;
        bool                rising2 = true;

        bool                holdControl[OSCILLATORS_COUNT];

};

#endif