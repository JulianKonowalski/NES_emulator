#ifndef APU_H
#define APU_H

#include "IO/Window.h"

#include "NES/APU/Oscillator.h"

class APU {
public:

    APU(Window* window, const unsigned int& sampleRate);
    ~APU(void);

    void update(void* buffer, unsigned int frames);


    //FOR TESTING
    void setOscFreq(const float& freq) { 
        mTriOscillator.setFrequency(freq);
        mOscillator.setFrequency(freq); 
    }
    float getOscFreq(void) { return mOscillator.getFrequency(); }


private:

    SinOscillator mOscillator;

    TriOscillator mTriOscillator;
    NoiseOscillator mNoiseOscillator;

    short* mAudioBuffer;
    short mAudioBufferSize;

};

#endif // !APU
