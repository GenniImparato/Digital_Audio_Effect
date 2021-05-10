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

        virtual void            writeNextBuffer(float* inBuff, float* outBuff);
        virtual void            postWrite(){};

        void                    setControlFromPot(unsigned int control, unsigned int value);
        EffectControl*          getControl(int control);
        
        void                    setName(const std::string &name);
        std::string             getName();

    protected:
        EffectControl           controls[POTS_COUNT];

    private:
        std::string             name;
};


#endif
