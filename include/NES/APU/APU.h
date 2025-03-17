#ifndef APU_H
#define APU_H

#include <cstdint>

#include "NES/APU/Oscillator.h"
#include "NES/APU/OscLUT.h"
#include "IO/Window.h"

static constexpr float CPU_CLOCK_SPEED = 1789773.0f;
static constexpr float MAX_AMPLITUDE = 15.0f;

enum VOL_MASK {
    DUTY =      0b11000000,
    LOOPING =   0b00100000,
    CONSTVOL =  0b00010000,
    VOLUME =    0b00001111
};

enum SWEEP_MASK {
    ENABLE =    0b10000000,
    PERIOD =    0b01110000,
    DIRECTION = 0b00001000,
    SHIFT =     0b00000111
};

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
    DMC_LEN =       0x4013,
    MODE =          0x4017
};


/* APU OSCILLATORS */

/*
* This class may be a bit confusing
* but basically it's an overlay
* that I use on top of my oscillators
* to accomodate their functionality
* to how NES passes and stores info
* about notes to play
*/

class APUPulse : public PulseOscillator {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    APUPulse(void);
    APUPulse(const unsigned int& sampleRate);

    void setEnabled(const bool& enabled) { mIsEnabled = enabled; }
    void setVolumeSettings(const Byte& settings);

    void setNoteLength(const Byte& length);
    void setSweepSettings(const Byte& settings);
    void setFrequency(const Byte& frequency, const bool& highByteWrite);

    void updateEnvelope(void);
    void updateLength(void);
    void updateSweep(void);

    bool isEnabled(void) { return mIsEnabled; }
    bool isLooping(void) { return mIsLooping; }
    bool isSweeping(void) { return mIsSweeping; }
    bool hasConstantVolume(void) { return mHasConstantVolume; }

    Byte getNoteLength(void) { return mNoteLength; }
    Word getInitialFreqency(void) { return mInitialFreqency; }
    Byte getInitialAmplitude(void) { return mInitialAmplitude; }

    float process(void);

private:

    bool mIsEnabled;
    bool mIsLooping;
    bool mIsSweeping;
    bool mSweepDown;
    bool mHasConstantVolume;

    //all are stored in NES notation
    Word mInitialFreqency;

    Byte mDivider;
    Byte mNoteLength;
    Byte mSweepPeriod;
    Byte mSweepShift;
    Byte mSweepClock;
    Byte mInitialAmplitude;
    Byte mCurrentAmplitude;

};




/* APU */

class APU {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    APU(Window* window, const unsigned int& sampleRate);
    ~APU(void);

    void clock(void);

    Byte readRegister(const Word& address);
    void writeRegister(const Byte& data, const Word& address);
    void update(void* buffer, unsigned int frames);


private:

    void writePulseVolume(const Byte& data, const Byte& oscIdx);
    void writePulseSweep(const Byte& data, const Byte& oscIdx);
    void writePulseLo(const Byte& data, const Byte& oscIdx);
    void writePulseHi(const Byte& data, const Byte& oscIdx);

    float getSample(void);

    OscLUT mOscLUT;

    APUPulse mPulseOscillator[2];
    TriOscillator mTriOscillator;
    NoiseOscillator mNoiseOscillator;

    Byte mMode;

    unsigned short mCycles;

    short* mAudioBuffer;
    unsigned short mAudioBufferSize;

};

#endif // !APU
