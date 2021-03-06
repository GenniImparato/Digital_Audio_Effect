#include "AudioChain.h"
#include "DAC_Driver/DAC_Driver.h"
#include "ADC_Driver/ADC_Driver.h"
#include "LedMatrix_Driver/LedMatrix_Driver.h"

using namespace miosix;

float                   AudioChain::adcBuff[AUDIO_BUFFERS_SIZE];

Mutex                   AudioChain::ctrlMutex;
Mutex                   AudioChain::adcBuffMutex;
Mutex                   AudioChain::effectMutex;

Thread*                 AudioChain::potThread = nullptr;
Thread*                 AudioChain::readADCThread = nullptr;
Thread*                 AudioChain::writeDACThread = nullptr;

AudioEffect*            AudioChain::activeEffect = nullptr;
int                     AudioChain::activeSource = 0;
AudioEffect*            AudioChain::synth = nullptr;
bool                    AudioChain::controlSynthFlag = false;
bool                    AudioChain::lastControlSynthFlag = false;
int                     AudioChain::effectChangedTimer = 0;
int                     AudioChain::refreshLastPotTimer = 0;
int                     AudioChain::lastPotChanged = -1;


void AudioChain::init()
{
    synth = new Synthesizer();

    nextEffect();
    //nextSource();
}

/*AudioChain::~AudioChain()
{
    readADCThread->terminate();
    writeDACThread->terminate();
    potThread->terminate();

    //wait for thread termination before deleting object
    //fix crash on delete
    while(Thread::exists(readADCThread) && Thread::exists(writeDACThread) && Thread::exists(potThread))
        Thread::yield();
}*/

void AudioChain::startThreads()
{
    readADCThread = Thread::create(&AudioChain::readADCThreadMain, 2048, 3);
    writeDACThread = Thread::create(&AudioChain::writeDACThreadMain, 2048, 3);
    potThread = Thread::create(&AudioChain::potThreadMain, 2048, 0);
}


void AudioChain::nextEffect()
{
    static int count = -1;
    count++;
    if(count>=AUDIO_EFFECTS_COUNT)
        count = 0;

    {
        Lock<Mutex> lock(effectMutex);

        if(activeEffect != nullptr)
            delete activeEffect;

        switch(count)
        {
            case 0:
                activeEffect = new AudioEffect();
                break;
            case 1:
                activeEffect = new Delay();
                break;
            case 2:
                activeEffect = new Filter();
                break;
            case 3:
                activeEffect = new Tremolo();
                break;
        }

        {
            Lock<Mutex> lock(ctrlMutex);
            if(controlSynthFlag)
                controlSynthFlag = false;
        }

        effectChangedTimer = 100;
        lastPotChanged = -1;

        LedMatrix_Driver::setString(std::string("FX   ") + activeEffect->getName());
    }
}

void AudioChain::nextSource()
{
    Lock<Mutex> lock(effectMutex);
    Lock<Mutex> lock2(ctrlMutex);

    activeSource++;

    if(activeSource>=AUDIO_SOURCES_COUNT)
        activeSource = ADC_SOURCE;

    if(activeSource == SYNTH_SOURCE)
    {
        controlSynthFlag = true;
        LedMatrix_Driver::setString(std::string("SRC  SYNTH"));
    }
    else if(activeSource == ADC_SOURCE)
    {
        controlSynthFlag = false;
        LedMatrix_Driver::setString(std::string("SRC  ADC"));
    }

    effectChangedTimer = 100;
}


void AudioChain::controlSynth()
{
    Lock<Mutex> lock(ctrlMutex);
    lastControlSynthFlag = controlSynthFlag;
    controlSynthFlag = true;
}

void AudioChain::controlEffect()
{
    Lock<Mutex> lock(ctrlMutex);
    lastControlSynthFlag = controlSynthFlag;
    controlSynthFlag = false;
}

bool  AudioChain::isSynthControlled()
{
    return controlSynthFlag;
}



void AudioChain::readADCLoop()
{
    const unsigned short* inBuff = ADC_Driver::getReadableBuffer(readADCThread);

    {   
        Lock<Mutex> dLock(adcBuffMutex);
        convertAudioBuffer_AdcToDsp(inBuff, adcBuff);
    }
    //printf("Buffer[%d] = %d\n", count, tmpBuff[100]);

    ADC_Driver::bufferEmptied();
}

void AudioChain::writeDACLoop()
{
    static float tmpBuff[AUDIO_BUFFERS_SIZE];

    /*for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
        tmpBuff[i] = 2.0*i/AUDIO_BUFFERS_SIZE -1.0 ;*/

    {
        Lock<Mutex> dLock2(effectMutex);
        if(activeSource == ADC_SOURCE)
        {
            Lock<Mutex> dLock(adcBuffMutex);
            
            activeEffect->writeNextBuffer(adcBuff, tmpBuff);    
        }
        else if(activeSource == SYNTH_SOURCE)
        {
            synth->writeNextBuffer(nullptr, tmpBuff);
            activeEffect->writeNextBuffer(tmpBuff, tmpBuff);
        }
    }

    //suspend thread and waits if no buff
    unsigned short* outBuff = DAC_Driver::getWritableBuffer(writeDACThread);
    convertAudioBuffer_DspToDac(tmpBuff, outBuff);
    DAC_Driver::bufferFilled();

    {
        Lock<Mutex> lock(effectMutex);
        activeEffect->postWrite();
        synth->postWrite();
    }
}

void AudioChain::convertAudioBuffer_AdcToDsp(const unsigned short* inBuff, float* outBuff)
{
    for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
        outBuff[i] = 4.0*((float)inBuff[i])/4069.0 - 2.0;
}

void AudioChain::convertAudioBuffer_DspToDac(float* inBuff, unsigned short* outBuff)
{
    for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
    {
        int out = (int)(0.5*(inBuff[i]+1.0)*45000.0);

        if(out > 65535)
            outBuff[i] = 65535;
        else if(out < 0)
            outBuff[i] = 0;
        else
            outBuff[i] = (unsigned short)out;
    }
}


void AudioChain::potLoop()
{
    for(int i=0; i<POTS_COUNT; i++)
    {
        unsigned short val = ADC_Driver::filterConversionsPot(i);

        Lock<Mutex> lock(effectMutex);

        AudioEffect *controlEffect;

        if(controlSynthFlag)
            controlEffect = synth;
        else
            controlEffect = activeEffect;

        controlEffect->setControlFromPot(i, val);

        if(effectChangedTimer==0)
        {
            EffectControl* ctrl = controlEffect->getControl(i);

            if(ctrl->isPotMoved(25))
            {
                LedMatrix_Driver::setString(ctrl->getMatrixString());
                lastPotChanged = i;
            }

            if(refreshLastPotTimer <= 0)
            {
                if(lastPotChanged != -1)
                    LedMatrix_Driver::setString(controlEffect->getControl(lastPotChanged)->getMatrixString());
                refreshLastPotTimer = 10;
            }
            refreshLastPotTimer--;
        }
        else
        {
            effectChangedTimer--;
        }

    }

    Thread::sleep(CONTROLS_REFRESH_PERIOD);
}

void AudioChain::writeDACThreadMain(void *param)
{
    while(!Thread::testTerminate())
    {
        writeDACLoop();
    }
}

void AudioChain::readADCThreadMain(void *param)
{
    while(!Thread::testTerminate())
    {
        readADCLoop();
    }
}

void AudioChain::potThreadMain(void *param)
{
    while(!Thread::testTerminate())
    {
        potLoop();
    }
}