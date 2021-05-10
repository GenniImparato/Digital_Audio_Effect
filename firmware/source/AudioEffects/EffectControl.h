#ifndef CONTROL___H___
#define CONTROL___H___

#include "../utils.h"
#include <math.h>
#include <string>
#include <vector>

class EffectControl
{
     public:
          EffectControl()            {iVal = 0; fVal = 0;};
          virtual ~EffectControl()   
          {
               choiseNames.clear();
          };

          void setValueFromPot(unsigned short adcVal)
          {
               this->lastPotVal = this->potValue;
               this->potValue = adcVal;
               this->lastConversionF = convertedF;
               this->convertedI = false;
               this->convertedF = false;
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

          void setName(const std::string &name)
          {
               this->name = name;
          }

          void setFloatDigits(int fDigits)
          {
               this->fDigits = fDigits;
          }

          void addChoiseName(std::string name)
          {
               choiseNames.push_back(name);
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

          std::string getMatrixString()
          {
               char buff[9];
               if(convertedF)
                    sprintf(buff, "%s%.*f", name.c_str(), fDigits, fVal);
               else if(convertedI)
               {
                    if(choiseNames.size()>iVal)
                         sprintf(buff, "%s%s", name.c_str(), choiseNames[iVal].c_str());
                    else sprintf(buff, "%s%d", name.c_str(), iVal);
               }
               else if(lastConversionF)
                    sprintf(buff, "%s%.*f", name.c_str(), fDigits, fLastVal);
               else
               {
                    if(choiseNames.size()>iLastVal)
                         sprintf(buff, "%s%s", name.c_str(), choiseNames[iLastVal].c_str());
                    else sprintf(buff, "%s%d", name.c_str(), iLastVal);
               }

               return std::string(buff);
          }

     private:
          unsigned short           potValue = 2000;                   
          unsigned short           lastPotVal=0;
          float                    fVal=0;
          float                    fLastVal=0;
          int                      iVal=0;
          int                      iLastVal=0;
          bool                     convertedF=false;
          bool                     convertedI=false;
          bool                     lastConversionF=false;
          std::string              name="CTRL ";
          int                      fDigits=0;
          std::vector<std::string> choiseNames;
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