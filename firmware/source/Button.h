#ifndef BUTTON__H__
#define BUTTON__H__

#include <miosix.h>
#include "AudioChain.h"

#define BUTTONS_REFRESH_PERIOD       50 //ms
#define BUTTONS_ANTIBOUNCE_DURATION    2 //ticks
#define BUTTONS_HOLD_TIME              10 //ticks


//button with pull-up resistor (triggers on 0)
template<int P, unsigned char N>
class Button
{
    public: 
        Button()
        {
            miosix::Gpio<P,N>::mode(miosix::Mode::INPUT);
        }

        void onClick()
        {
            AudioChain::nextEffect();
        };

        void onHold()   
        {
            AudioChain::nextSource();
        };

        //to be called in main loop
        void check()
        {
            ticks++;
                        
            if(!antiBounce)
            {         
                if(readValue())
                {
                    antiBounce = true;
                    ticks = 0;
                }
            }
            else
            {
                if(ticks> BUTTONS_HOLD_TIME)
                {
                  if(!onHoldTriggered)
                  {
                    onHold();
                    onHoldTriggered = true;
                  }
                }

                if((ticks> BUTTONS_ANTIBOUNCE_DURATION) && !readValue())
                {
                    if(ticks < BUTTONS_HOLD_TIME)
                        onClick();
                    
                    antiBounce = false;
                    onHoldTriggered = false;
                }
            }

            miosix::Thread::sleep(BUTTONS_REFRESH_PERIOD);
        }


      private:   
        bool            antiBounce = false;
        bool            onHoldTriggered = false;
        int             oldTicks = 0;
        int             ticks = 0;

        bool readValue()
        {
            return miosix::Gpio<P,N>::value();
        }
    
};

#endif
