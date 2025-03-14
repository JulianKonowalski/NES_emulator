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
        mPulseOscillator[0].setFrequency(freq);
        mPulseOscillator[1].setFrequency(freq);
    }

    void setOscShift(const int& shift) {
        mPulseOscillator[0].setPhaseShift(shift);
        mPulseOscillator[1].setPhaseShift(shift);
    }

    float getOscFreq(void) { return mPulseOscillator[0].getFrequency(); }
    int getOscShift(void) { return mPulseOscillator[0].getPhaseShift(); }


private:

    PulseOscillator mPulseOscillator[2];
    TriOscillator mTriOscillator;
    NoiseOscillator mNoiseOscillator;

    short* mAudioBuffer;
    short mAudioBufferSize;

};

#endif // !APU
