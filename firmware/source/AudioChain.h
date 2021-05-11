#ifndef AUDIO_CHAIN__H__
#define AUDIO_CHAIN__H__

#include <miosix.h>
#include "config.h"
#include "utils.h"
#include "AudioEffects/AudioEffect.h"
#include "AudioEffects/Synthesizer.h"
#include "AudioEffects/Delay.h"



#define CONTROLS_COUNT              4
#define CONTROLS_REFRESH_PERIOD     20 // ms

#define AUDIO_SOURCES_COUNT         2
#define ADC_SOURCE                  0
#define SYNTH_SOURCE                1

#define AUDIO_EFFECTS_COUNT         2


class AudioChain
{
    public:
        static void                     init();
        static void                     startThreads();

        static void                     nextEffect();
        static void                     nextSource();

        static void                     controlSynth();
        static void                     controlEffect();
        static bool                     isSynthControlled();

    private:
        AudioChain();

        //buffer shared by reader/writer, syncronized with buffMutex
        static  float                   adcBuff[AUDIO_BUFFERS_SIZE];

        static  miosix::Mutex           ctrlMutex;
        static  miosix::Mutex           adcBuffMutex;
        static  miosix::Mutex           effectMutex;

        //periodic thread that reads potentiometers, low priority
        static  miosix::Thread*         potThread;
        //high priority
        static  miosix::Thread*         readADCThread;
        //high priority
        static  miosix::Thread*         writeDACThread;

        static  AudioEffect*            activeEffect;
        static  int                     activeSource;
        static  AudioEffect*            synth;
        static  bool                    controlSynthFlag;
        static  bool                    lastControlSynthFlag;
        static  int                     effectChangedTimer;
        static  int                     refreshLastPotTimer;
        static  int                     lastPotChanged;

        //effect main dsp loop (not static)
        static  void                    readADCLoop();
        static  void                    writeDACLoop();
        static  void                    potLoop();

        static  void                    convertAudioBuffer_AdcToDsp(const unsigned short* inBuff, float* outBuff);
        static  void                    convertAudioBuffer_DspToDac(float* inBuff, unsigned short* outBuff);

        //threads entry points, param is a pointer to AudioEffect
        static  void                    readADCThreadMain(void *param);
        static  void                    writeDACThreadMain(void *param);
        static  void                    potThreadMain(void *param);
};

#endif
