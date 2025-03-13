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
        mPulseOscillator1.setFrequency(freq);
        mPulseOscillator2.setFrequency(freq);
    }

    void setOscShift(const int& shift) {
        mPulseOscillator1.setPhaseShift(shift);
        mPulseOscillator2.setPhaseShift(shift);
    }

    float getOscFreq(void) { return mPulseOscillator1.getFrequency(); }
    int getOscShift(void) { return mPulseOscillator1.getPhaseShift(); }


private:

    PulseOscillator mPulseOscillator1;
    PulseOscillator mPulseOscillator2;
    TriOscillator mTriOscillator;
    NoiseOscillator mNoiseOscillator;

    short* mAudioBuffer;
    short mAudioBufferSize;

};

#endif // !APU
