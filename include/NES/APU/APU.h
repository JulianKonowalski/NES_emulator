#ifndef APU_H
#define APU_H

#include "IO/Window.h"

#include <cstdint>

#include "NES/APU/Oscillator.h"

enum APU_REGISTER {
    SQ1_VOL =       0x4000,
    SQ1_SWEEP =     0x4001,
    SQ1_LO =        0x4002,
    SQ1_HI =        0x4003,
    SQ2_VOL =       0x4004,
    SQ2_SWEEP =     0x4005,
    SQ2_LO =        0x4006,
    SQ2_HI =        0x4007,
    TRI_LINEAR =    0x4008,
    TRI_LO =        0x400A,
    TRI_HI =        0x400B,
    NOISE_VOL =     0x400C,
    NOISE_LO =      0x400E,
    NOISE_HI =      0x400F,
    DMC_FREQ =      0x4010,
    DMC_RAW =       0x4011,
    DMC_START =     0x4012,
    DMC_LEN =       0x4013
};

class APU {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    APU(Window* window, const unsigned int& sampleRate);
    ~APU(void);

    Byte readRegister(const Word& address);
    void writeRegister(const Byte& data, const Word& address);
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

    Word mPulseFrequency[2];

    short* mAudioBuffer;
    short mAudioBufferSize;

};

#endif // !APU
