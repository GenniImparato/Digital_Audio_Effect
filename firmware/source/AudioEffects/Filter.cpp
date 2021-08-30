#include "Filter.h"
#include <math.h>       /* exp */

//float Delay::delayBuff[DELAY_SAMPLES_COUNT] = {};

Filter::Filter()
{
	controls[0].setName(std::string("CUTOF"));
	controls[0].setFloatDigits(2);

	controls[1].setName(std::string("RESNC"));
	controls[1].setFloatDigits(2);

	controls[2].setName(std::string("MODE "));
	controls[2].addChoiseName(std::string("LOWPS"));
    controls[2].addChoiseName(std::string("HIGPS"));
    controls[2].addChoiseName(std::string("BNDPS"));

	setName(std::string("FILTR"));

	calculateFeedbackAmount();
}

void Filter::writeNextBuffer(float* inBuff, float* outBuff)
{
	switch (mode) 
	{
	    case FILTER_MODE_LOWPASS:
	        for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	        {
			   	buf0 += cutoff * (inBuff[i] - buf0 + feedbackAmount * (buf0 - buf1));
			    buf1 += cutoff * (buf0 - buf1);
			    buf2 += cutoff * (buf1 - buf2);
			    buf3 += cutoff * (buf2 - buf3);
			    outBuff[i] =  buf3;
			}
		break;

	    case FILTER_MODE_HIGHPASS:
	        for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	        {
			   	buf0 += cutoff * (inBuff[i] - buf0 + feedbackAmount * (buf0 - buf1));
			    buf1 += cutoff * (buf0 - buf1);
			    buf2 += cutoff * (buf1 - buf2);
			    buf3 += cutoff * (buf2 - buf3);
			    outBuff[i] =  inBuff[i] - buf3;
			}
		break;

	    case FILTER_MODE_BANDPASS:
	        for(int i=0; i<AUDIO_BUFFERS_SIZE; i++)
	        {
			   	buf0 += cutoff * (inBuff[i] - buf0 + feedbackAmount * (buf0 - buf1));
			    buf1 += cutoff * (buf0 - buf1);
			    buf2 += cutoff * (buf1 - buf2);
			    buf3 += cutoff * (buf2 - buf3);
			    outBuff[i] =  buf0 - buf3;
			}
			break;
	}
}

void Filter::postWrite()
{
	cutoff = controls[0].getFloatValue(0, 0.99);
	resonance = controls[1].getFloatValue(0, 0.99);
	mode = static_cast<FilterMode>(controls[2].getIntValue(0, 3));
}



