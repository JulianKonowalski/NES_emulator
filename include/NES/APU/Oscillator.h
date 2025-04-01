#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cstdint>

static constexpr float MAX_AMPLITUDE = 15.0f;
static constexpr float CPU_CLOCK_SPEED = 1789773.0f;

class APUOscillator {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    APUOscillator(void);

    void setEnabled(const bool& enabled);
    void setLooping(const bool& isLooping);
    void setConstantVolume(const bool& hasConstantVolume);
    void setNoteLength(const Byte& length);
    void setAmplitude(const Byte& amplitude);
    void setFrequency(const Byte& frequency, const bool& highByte);
    void setSampleRate(const unsigned int& sampleRate);

    bool isEnabled(void) { return mIsEnabled; }
    bool isLooping(void) { return mIsLooping; }
    bool hasConstantVolume(void) { return mHasConstantVolume; }
    Byte getAmplitude(void) { return mCurrentAmplitude; }
    Word getFrequency(void) { return mFrequency; }
    unsigned int getSampleRate(void) { return mSampleRate; }

    void updateLength(void);
    void updateVolume(void);

    float process(void);

    inline static constexpr Byte DEFAULT_AMPLITUDE = 0;
    inline static constexpr Word DEFAULT_FREQUENCY = 0xFFFF;
    inline static constexpr unsigned int DEFAULT_SAMPLE_RATE = 44100;

protected:

    bool mIsEnabled;
    bool mIsLooping;
    bool mHasConstantVolume;

    Byte mDivider;
    Byte mNoteLength;
    Byte mInitialAmplitude;
    Byte mCurrentAmplitude;
    Word mFrequency;

    float mRealAmplitude;
    unsigned int mSampleRate;

};

class APUNoise : public APUOscillator {
public:

    APUNoise(void);

    void setModeFlag(const bool& state) { mMode = state; }
    bool getModeFlag(void) { return mMode; }

    void setSampleRate(const unsigned int& sampleRate);

    float process(void);

private:

    void updateRegister(void);

    bool mMode;
    Word mLFSR;

    float mOffset;
    float mAngle;

};

class APUTri : public APUOscillator {
public:

    APUTri(void);

    void setReloadFlag(void) { mReloadFlag = true; }    //it can only be set to true
    void setLinearCounter(const Byte& counterValue);

    void updateLength(void);
    void updateLinearCounter(void);

    void setFrequency(const Byte& frequency, const bool& highByte);
    void setSampleRate(const unsigned int& sampleRate);

    float process(void);

private:

    bool mReloadFlag;
    Byte mLinearReload;
    Byte mLinearCounter;

    float mRealFrequency;
    float mOffset;
    float mAngle;

    static constexpr float MAX_OUTPUT_VALUE = 15.0f;
    static constexpr Byte NUM_OUTPUT_VALUES = 32;
    static constexpr float OUTPUT_VALUES[32] = {
        15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
    };

};

class APUPulse : public APUOscillator {
public:

    APUPulse(void);

    void setDutyCycle(const float& dutyCycle);
    void setFrequency(const Byte& frequency, const bool& highByteWrite);
    void setSampleRate(const unsigned int& sampleRate);

    void setSweepEnabled(const bool& isSweeping) { mIsSweeping = isSweeping; }
    void setSweepDown(const Byte& sweepDown) { mSweepDown = sweepDown; }
    void setSweepShift(const Byte& shift) { mSweepShift = shift; }
    void setSweepPeriod(const Byte& period);

    void updateSweep(void);

    float process(void);

    static constexpr float DEFAULT_DUTY_CYCLE = 0.5f;

private:

    bool mIsSweeping;
    bool mSweepDown;

    Byte mSweepShift;
    Byte mSweepPeriod;
    Byte mSweepClock;

    float mRealFrequency;
    float mDutyCycle;
    float mOffset;
    float mAngle;

};


#endif // !OSCILLATOR_H
