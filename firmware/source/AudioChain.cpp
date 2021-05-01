#include "AudioChain.h"
#include "DAC_Driver/DAC_Driver.h"
#include "ADC_Driver/ADC_Driver.h"

using namespace miosix;

unsigned short          AudioChain::adcBuff[AUDIO_BUFFERS_SIZE];

Mutex                   AudioChain::ctrlMutex;
Mutex                   AudioChain::buffMutex;

Thread*                 AudioChain::potThread = nullptr;
Thread*                 AudioChain::readADCThread = nullptr;
Thread*                 AudioChain::writeDACThread = nullptr;

AudioEffect*            AudioChain::activeEffect = nullptr;
int                     AudioChain::activeSource = 0;
AudioEffect*            AudioChain::synth = nullptr;
bool                    AudioChain::controlSynth = true;

void AudioChain::init()
{
    synth = new Synthesizer();

    nextEffect();
    nextEffect();
    //nextEffect();

    nextSource();
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
    potThread = Thread::create(&AudioChain::potThreadMain, STACK_MIN, 0);
}


void AudioChain::nextEffect()
{
    static int count = -1;
    count++;
    if(count>=AUDIO_EFFECTS_COUNT)
        count = 0;

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
    }
}

void AudioChain::nextSource()
{
    int old = activeSource;
    activeSource++;

    if(activeSource>=AUDIO_SOURCES_COUNT)
        activeSource = 0;
}




void AudioChain::readADCLoop()
{
    const unsigned short* inBuff = ADC_Driver::getReadableBuffer(readADCThread);

    {   
        Lock<Mutex> dLock(buffMutex);
        for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
            adcBuff[i] = map(inBuff[i], 1, 4096, 1, 63000);

    }
    //printf("Buffer[%d] = %d\n", count, tmpBuff[100]);

    ADC_Driver::bufferEmptied();
}

void AudioChain::writeDACLoop()
{
    //suspend thread and waits if no buff
    unsigned short* outBuff = DAC_Driver::getWritableBuffer(writeDACThread);

    if(activeSource == ADC_SOURCE)
    {
        Lock<Mutex> dLock(buffMutex);
        activeEffect->writeNextBuffer(outBuff, adcBuff);    
    }
    else if(activeSource == SYNTH_SOURCE)
    {
        static unsigned short tmpBuff[AUDIO_BUFFERS_SIZE];
        synth->writeNextBuffer(tmpBuff, nullptr);
        activeEffect->writeNextBuffer(outBuff, tmpBuff);
    }
    DAC_Driver::bufferFilled();

    {
        Lock<Mutex> lock(ctrlMutex);
        activeEffect->postWrite();
        synth->postWrite();
    }
}


void AudioChain::potLoop()
{
    {
        Lock<Mutex> lock(ctrlMutex);
        for(int i=0; i<CONTROLS_COUNT; i++)
        {
            unsigned short val = ADC_Driver::singleConversionPot(i);
            
            if(controlSynth)
                synth->setControlFromPot(i, val);
            else
                activeEffect->setControlFromPot(i, val);
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