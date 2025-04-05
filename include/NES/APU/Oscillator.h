#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cstdint>

/** Max amplitude of the NES system */
static constexpr float MAX_AMPLITUDE = 15.0f;

/** NES' CPU clock speed */
static constexpr float CPU_CLOCK_SPEED = 1789773.0f;

/**
* Base class for all APU
* oscillators.
* 
* @see APU
*/
class APUOscillator {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    /**
    * Class constructor. Initializes
    * the class instance with default
    * values.
    */
    APUOscillator(void);

    /**
    * Enables or disables the
    * oscillator.
    * 
    * @param enabled flag if the
    *   oscillator should be enabled
    * 
    * @see mIsEnabled
    */
    void setEnabled(const bool& enabled);

    /**
    * Sets if the oscillator
    * should be looping.
    * 
    * @param isLooping flag if the
    *   oscillator should be looping
    * 
    * @see mIsLooping
    */
    void setLooping(const bool& isLooping);

    /**
    * Sets if the oscillator has
    * constant volume.
    * 
    * @param hasConstantVolume state
    *   of the constant volume option
    * 
    * @see mHasConstantVolume
    */
    void setConstantVolume(const bool& hasConstantVolume);

    /**
    * Sets the note length
    * played by the oscillator.
    * 
    * @param length oscillator's
    *   note length
    * 
    * @see mNoteLength
    */
    void setNoteLength(const Byte& length);

    /**
    * Sets the oscillator's volume.
    * 
    * @param amplitude oscillator's 
    *   volume
    * 
    * @see mInitialAmplitude
    * @see mCurrentAmplitude
    * @see mRealAmplitude
    */
    void setAmplitude(const Byte& amplitude);

    /**
    * Sets the oscillator's frequency.
    * 
    * @param frequency oscillator's frequency
    * @param highByte flag indicating if the
    *   current write is reffering to the high
    *   or low byte of the frequency value
    * 
    * @see mFrequency
    */
    void setFrequency(const Byte& frequency, const bool& highByte);

    /**
    * Sets the oscillator's sample rate.
    * 
    * @param sampleRate oscillator's sample rate
    * 
    * @see mSampleRate
    */
    void setSampleRate(const unsigned int& sampleRate);

    /**
    * Returns the information if the
    * oscillator is enabled.
    * 
    * @return true if is enabled
    * 
    * @see mIsEnabled
    */
    bool isEnabled(void) { return mIsEnabled; }

    /**
    * Returns the information if the
    * oscillator is looping.
    * 
    * @return true if is looping
    * 
    * @see mIsLooping
    */
    bool isLooping(void) { return mIsLooping; }

    /**
    * Returns the information if the
    * oscillator has constant volume.
    * 
    * @return true if has constant volume
    * 
    * @see mHasConstantVolume
    */
    bool hasConstantVolume(void) { return mHasConstantVolume; }

    /**
    * Returns the oscillator's current
    * volume.
    * 
    * @return oscillator's current voluem
    * 
    * @see mCurrentAmplitude
    */
    Byte getAmplitude(void) { return mCurrentAmplitude; }

    /**
    * Returns the oscillator's frequency
    * 
    * @return oscillator's frequency
    * 
    * @see mFrequency
    */
    Word getFrequency(void) { return mFrequency; }

    /**
    * Returns the oscillator's sample rate
    * 
    * @return oscillator's sample rate
    * 
    * @see mSampleRate
    */
    unsigned int getSampleRate(void) { return mSampleRate; }

    /**
    * Updates the note length.
    * If the note length reaches
    * 0 or the oscillator is not enabled
    * the oscillator is muted. If the 
    * note length is already 0 or the
    * oscillator is looping the method
    * returns.
    * 
    * @see mNoteLength
    * @see mIsLooping
    * @see mIsEnabled
    */
    void updateLength(void);

    /**
    * Decrements the divider and
    * updates the oscillator volume
    * when it reaches 0. If the 
    * oscillator has constant volume 
    * the method returns. If the 
    * volume reaches 0 and the oscillator 
    * is looping the volume is reset to 
    * the initial value. Otherwise the
    * oscillator is muted.
    * 
    * @see mDivider
    * @see mInitialAmplitude
    * @see mCurrentAmplitude
    * @see mRealAmplitude
    * @see mHasConstantVolume
    */
    void updateVolume(void);

    /**
    * Clocks the oscillator
    * and returns the next
    * sample value.
    * 
    * @return generated audio sample
    */
    float process(void);

    /** Default oscillator amplitude */
    inline static constexpr Byte DEFAULT_AMPLITUDE = 0;

    /** Default oscillator frequency */
    inline static constexpr Word DEFAULT_FREQUENCY = 0xFFFF;

    /** Default oscillator sample rate */
    inline static constexpr unsigned int DEFAULT_SAMPLE_RATE = 44100;

protected:

    /** Flag indicating if the oscillator is enabled */
    bool mIsEnabled;

    /** Flag indicating if the oscillator is looping */
    bool mIsLooping;

    /** Flag indicating if the oscillator has constant volume */
    bool mHasConstantVolume;

    /** Counter for updating the oscillator's volume */
    Byte mDivider;

    /** Length of the note played by the oscillator */
    Byte mNoteLength;

    /** Initial amplitude of the oscillator */
    Byte mInitialAmplitude;

    /** Current amplitude of the oscillator */
    Byte mCurrentAmplitude;
    
    /** Frequency of the oscillator */
    Word mFrequency;

    /** 
    * Current amplitude of the oscillator 
    * in the range of 0.0f - 1.0f.
    */
    float mRealAmplitude;

    /** Sample rate of the oscillator */
    unsigned int mSampleRate;

};

/**
* Class emulating the
* APU noise oscillator.
*/
class APUNoise : public APUOscillator {
public:

    /**
    * Class constructor. It initializes
    * the class instance with default values.
    */
    APUNoise(void);

    /**
    * Sets the mode of the oscillator. 
    * Noise oscillator has two modes, which
    * differ in the sound character.
    * 
    * @param state flag determinig the 
    *   oscillator's operating mode
    * 
    * @see mMode
    */
    void setModeFlag(const bool& state) { mMode = state; }

    /**
    * Returns the current operating
    * mode of the oscillator.
    * 
    * @return current operating mode
    *   of the oscillator
    * 
    * @see mMode
    */
    bool getModeFlag(void) { return mMode; }

    /**
    * Sets the sample rate of
    * the oscillator.
    */
    void setSampleRate(const unsigned int& sampleRate);

    /**
    * Clocks the oscillator
    * and returns the next
    * sample value stored
    * in the internal shift
    * register.
    * 
    * @return generated audio sample
    */
    float process(void);

private:

    /**
    * Updates the internal
    * shift register by
    * performing a bit shift.
    * This operation is performed
    * only when a certain amount
    * of time has passed. This 
    * is dictated by the oscillator's
    * frequency.
    * 
    * @see mLFSR
    * @see mFrequency
    * @see mAngle
    * @see mOffset
    */
    void updateRegister(void);

    /**
    * Flag determining the
    * operating mode of the 
    * oscillator. If set
    * to false the signal
    * is generated by XORing
    * 1st bit of the shift
    * register with the 2nd bit,
    * otherwise the 1st bit is
    * XORed with the 6th bit.
    */
    bool mMode;

    /**
    * A shift register containing
    * the signal generated by the
    * noise function.
    */
    Word mLFSR;

    /**
    * Increment that the
    * angle is changed by
    * every oscillator cycle.
    */
    float mOffset;

    /**
    * Angle of the oscillator.
    * It is treated as an argument
    * to the noise function determining
    * when to shift the shift register.
    */
    float mAngle;

};

/**
* Class emulating the
* APU triangle oscillator.
*/
class APUTri : public APUOscillator {
public:

    /**
    * Class constructor. It initializes
    * the class instance with default 
    * values.
    */
    APUTri(void);

    /**
    * Sets the reload flag
    * to true.
    * 
    * @see mReloadFlag
    */
    void setReloadFlag(void) { mReloadFlag = true; }

    /**
    * Sets the linear counter and
    * linear reload to the given value.
    * 
    * @param counterValue new linear
    *   counter value.
    * 
    * @see mLinearCounter
    * @see mLinearReload
    */
    void setLinearCounter(const Byte& counterValue);

    /**
    * Updates the note length.
    * If the note length reaches
    * 0 or the oscillator is not enabled
    * the oscillator is muted. If the 
    * note length is already 0 or the
    * oscillator is looping the method
    * returns.
    * 
    * @see mNoteLength
    * @see mIsLooping
    * @see mIsEnabled
    */
    void updateLength(void);

    /**
    * Updates linear counter. 
    * Works the same as updating 
    * the volume in base oscillator.
    * 
    * @see mLinearCounter
    * @see mLinearReload
    * 
    */
    void updateLinearCounter(void);

    /**
    * Sets the oscillator's 
    * frequency
    * 
    * @param frequency oscillator's
    *   frequency
    * @param hightByte flag to indicate
    *   if the write is reffering to the 
    *   high or low byte of the frequency
    * 
    * @see mFrequency
    * @see mOffset
    * @see mAngle
    */
    void setFrequency(const Byte& frequency, const bool& highByte);

    /**
    * Sets the oscillator's sample rate
    * 
    * @param sampleRate oscillator's
    *   sample rate
    * 
    * @see mSampleRate
    */
    void setSampleRate(const unsigned int& sampleRate);

    /**
    * Clocks the oscillator
    * and returns the next
    * sample value. The values
    * are fetched from the predefined
    * sample values.
    * 
    * @return fetched audio sample
    * 
    * @see OUTPUT_VALUES
    * @see mAngle
    * @see mOffset
    */
    float process(void);

private:

    /** 
    * Defines if the linear 
    * counter should be reloaded 
    */
    bool mReloadFlag;

    /** Linear counter reload value */
    Byte mLinearReload;

    /** Counts the APU cycles */
    Byte mLinearCounter;

    /** 
    * Current frequency of the 
    * oscillator given in hertz.
    */
    float mRealFrequency;
    float mOffset;
    float mAngle;

    /** Max NES' output amplitude */
    static constexpr float MAX_OUTPUT_VALUE = 15.0f;

    /** Amount of possible output sample values */
    static constexpr Byte NUM_OUTPUT_VALUES = 32;

    /** Predefined oscillator output values */
    static constexpr float OUTPUT_VALUES[32] = {
        15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
    };

};

class APUPulse : public APUOscillator {
public:

    /**
    * Class constructor. Initializes 
    * the class instance with default 
    * values.
    */
    APUPulse(void);

    /**
    * Sets the oscillator's
    * duty cycle.
    * 
    * @param dutyCycle value of the
    *   duty cycle
    *
    * @see mDutyCycle
    */
    void setDutyCycle(const float& dutyCycle);

    /**
    * Sets the oscillator's frequency.
    * 
    * @param frequency oscillator's frequency
    * @param highByteWrite flag to determine
    *   if the write is referring to the high
    *   or low byte of the frequency
    * 
    * @see mFrequency
    * @see mRealFrequency
    */
    void setFrequency(const Byte& frequency, const bool& highByteWrite);

    /**
    * Sets the oscillator's sample rate
    * 
    * @param sampleRate oscillator's
    *   sample rate
    *   
    * @see mSampleRate
    */
    void setSampleRate(const unsigned int& sampleRate);

    /**
    * Enables or disables the oscillator's
    * sweep unit.
    * 
    * @param isSweeping flag to determine
    *   if the sweeping unit should be 
    *   enabled or disabled
    * 
    * @see mIsSweeping
    */
    void setSweepEnabled(const bool& isSweeping) { mIsSweeping = isSweeping; }

    /**
    * Sets the direction of the oscillator's
    * sweeping unit.
    * 
    * @param sweepDown flag to determine
    *   if the oscillator should be sweeping
    *   down
    * 
    * @see mSweepDown
    */
    void setSweepDown(const bool& sweepDown) { mSweepDown = sweepDown; }

    /**
    * Sets the speed of the oscillator's
    * sweeping unit.
    * 
    * @param shift sweep unit increment
    * 
    * @see mSweepShift
    */
    void setSweepShift(const Byte& shift) { mSweepShift = shift; }

    /**
    * Sets the length of the sweep and
    * restarts the sweep clock.
    * 
    * @param period duration of the sweep
    * 
    * @see mSweepPeriod
    * @see mSweepClock
    */
    void setSweepPeriod(const Byte& period);

    /**
    * Updates the sweep unit.
    */
    void updateSweep(void);

    /**
    * Clocks the oscillator
    * and returns the next
    * generated sample value.
    * 
    * @return generated audio sample
    * 
    * @see mAngle
    * @see mOffset
    */
    float process(void);

    /** Default duty cycle */
    static constexpr float DEFAULT_DUTY_CYCLE = 0.5f;

private:

    /** 
    * Flag to determine if the 
    * oscillator should be sweeping 
    */
    bool mIsSweeping;

    /** 
    * Flag to determine the 
    * direction of the sweeping unit 
    */
    bool mSweepDown;

    /** Sweeping  unit increment */
    Byte mSweepShift;

    /** Length of the sweep */
    Byte mSweepPeriod;

    /** Clock of the sweep unit */
    Byte mSweepClock;

    /** 
    * Current frequency of the 
    * oscillator given in hertz.
    */
    float mRealFrequency;

    /** Ratio between the length of high and low signal */
    float mDutyCycle;

    /**
    * Offset that the angle
    * is incremented by every
    * generated sample.
    */
    float mOffset;

    /**
    * Angle of the square
    * wave function.
    */
    float mAngle;

};


#endif // !OSCILLATOR_H
