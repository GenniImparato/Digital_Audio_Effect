#ifndef AUDIO_EFFECT_FILTER___H___
#define AUDIO_EFFECT_FILTER___H___

#include "AudioEffect.h"
	 	

class Filter	:public AudioEffect
{
	public:
		Filter();
		~Filter(){};

		//overriden methods
		void				writeNextBuffer(float* inBuff, float* outBuff);
		void				postWrite();

		enum FilterMode 
		{
	        FILTER_MODE_LOWPASS = 0,
	        FILTER_MODE_HIGHPASS,
	        FILTER_MODE_BANDPASS
    	};

	private:
	    float 				cutoff 	= 0.5;
	    float 				resonance = 0.5;
	    FilterMode 			mode = FILTER_MODE_BANDPASS;
	    float 				feedbackAmount;
	    float 				buf0 = 0;
	    float 				buf1 = 0;
	    float 				buf2 = 0;
	    float 				buf3 = 0;

	    inline void calculateFeedbackAmount() { feedbackAmount = resonance + resonance/(1.0 - cutoff); }

};
#endif