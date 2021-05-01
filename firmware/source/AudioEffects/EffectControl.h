#ifndef CONTROL___H___
#define CONTROL___H___

#include "../utils.h"
#include <math.h>

class EffectControl
{
     public:
          EffectControl()            {};
          virtual ~EffectControl()   {};

          void setValueFromPot(unsigned short adcVal)
          {
               this->lastPotVal = this->potValue;
               this->potValue = adcVal;
               convertedI = false;
               convertedF = false;
          }

          int getIntValue(int minVal, int maxVal)
          {
               if(!convertedI)
               {
                    iLastVal = iVal;
                    iVal = map(potValue, 0, 4096, minVal, maxVal);
                    convertedI = true;
               }

               return iVal;
          }

          float getFloatValue(float minVal, float maxVal)
          {
               if(!convertedF)
               {
                    fLastVal = fVal;
                    fVal = map_f(potValue, 0, 4096, minVal, maxVal);
                    convertedF = true;
               }

               return fVal;
          }

          /*bool isChangedFloat(float diff)
          {
               return (abs(fLastVal-fVal) >= diff);
          }*/

          bool isPotMoved(int diff)
          {
               return(abs(lastPotVal-potValue) >= diff);
          }

          bool isChangedInt(int diff)
          {
               return (abs(iLastVal-iVal) >= diff);
          }

     protected:
          unsigned short      potValue;
          unsigned short      lastPotVal=0;
          float               fVal=0;
          float               fLastVal=0;
          int                 iVal=0;
          int                 iLastVal=0;
          bool                convertedF=false;
          bool                convertedI=false;
};

/*
class SelectionControl   : public EffectControl
{
     public:
          SelectionControl(unsigned int choisesNum)  :EffectControl(0, choisesNum-1)    
          {
               /*this->choisesNum = choisesNum;
               this->stepSize = 4096/choisesNum;
          };
          virtual ~SelectionControl()                {};

     /*private:
          unsigned int choisesNum;
          unsigned int stepSize;

          void  convertValue()
          {
               value = potValue/stepSize;
          }
};*/

#endif