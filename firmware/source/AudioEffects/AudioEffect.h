#ifndef AUDIO_EFFECTS__H__
#define AUDIO_EFFECTS__H__

#include <miosix.h>
#include "EffectControl.h"
#include "../config.h"
#include "../utils.h"


//base class for audio effect
class AudioEffect
{
    public:
        AudioEffect();
        //terminates threads
        virtual ~AudioEffect();

        virtual void            preWrite(){};
        virtual void            writeNextBuffer(unsigned short* wrBuff, unsigned short* rdBuffer);
        virtual void            postWrite(){};

        void                    setControlFromPot(unsigned int control, unsigned int value);

    protected:
        EffectControl*          controls[POTS_COUNT];
};


#endif
