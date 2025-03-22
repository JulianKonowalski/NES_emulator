#include "NES/APU/Oscillator.h"

/******************/
/* APU OSCILLATOR */
/******************/

APUOscillator::APUOscillator(void) :
    mIsEnabled(false),
    mIsLooping(false),
    mHasConstantVolume(false),
    mDivider(0),
    mNoteLength(0),
    mInitialAmplitude(DEFAULT_AMPLITUDE),
    mCurrentAmplitude(DEFAULT_AMPLITUDE),
    mFrequency(DEFAULT_FREQUENCY),
    mRealAmplitude(0.0f),
    mSampleRate(DEFAULT_SAMPLE_RATE)
{}

void APUOscillator::setEnabled(const bool& enabled) {
    mIsEnabled = enabled;
}

void APUOscillator::setLooping(const bool& isLooping) {
    mIsLooping = isLooping;
}

void APUOscillator::setConstantVolume(const bool& hasConstantVolume) {
    mHasConstantVolume = hasConstantVolume;
}

void APUOscillator::setNoteLength(const Byte& length) {
    mNoteLength = length;
}

void APUOscillator::setAmplitude(const Byte& amplitude) {
    mInitialAmplitude   = amplitude;
    mCurrentAmplitude   = amplitude;
    mDivider            = amplitude;
    mRealAmplitude      = amplitude / MAX_AMPLITUDE;
}

void APUOscillator::setFrequency(const Byte& frequency, const bool& highByteWrite) {
    mFrequency = highByteWrite ? 
        (mFrequency & 0x00FF) | ((frequency & 0x7) << 8)
        : (mFrequency & 0xFF00) | frequency;
}

void APUOscillator::setSampleRate(const unsigned int& sampleRate) {
    if (sampleRate == 0) { return; }
    mSampleRate = sampleRate;
}

void APUOscillator::updateLength(void) {
    if (!mNoteLength) { return; }
    if (mIsLooping) { return; }
    if (!mIsEnabled || --mNoteLength == 0)
        this->setAmplitude(0);
}

void APUOscillator::updateVolume(void) {
    if (mHasConstantVolume) { return; }
    if (--mDivider == 0) {
        mDivider = mInitialAmplitude;
        if (--mCurrentAmplitude == 0)
            mCurrentAmplitude = (mNoteLength != 0) ? mInitialAmplitude : 0;
        mRealAmplitude = mCurrentAmplitude / MAX_AMPLITUDE;
    }
}

float APUOscillator::process(void) {
    return 0.0f;
}


/*************/
/* APU NOISE */
/*************/


APUNoise::APUNoise(void) :
    APUOscillator::APUOscillator(),
    mMode(false),
    mLFSR(1),
    mAngle(0.0f)
{
    mOffset = CPU_CLOCK_SPEED / mSampleRate;
}

void APUNoise::setSampleRate(const unsigned int& sampleRate) {
    APUOscillator::setSampleRate(sampleRate);
    mOffset = CPU_CLOCK_SPEED / mSampleRate;
}

void APUNoise::updateRegister(void) {
    mAngle += mOffset;
    if (mAngle < mFrequency) { return; }
    mAngle = 0.0f;
    Byte MSB = 0;
    Byte LSB = mLFSR & 0x1;
    mLFSR >>= 1;
    if (mMode) { MSB = LSB ^ (mLFSR & (1 << 4)); } 
    else { MSB = LSB ^ (mLFSR & 0x1); }
    mLFSR = mLFSR | (MSB << 14);
}

float APUNoise::process(void) {
    if (!mIsEnabled) { return 0.0f; }
    this->updateRegister();
    return mRealAmplitude * (mLFSR & 0x1);
}


/****************/
/* APU TIRANGLE */
/****************/


APUTri::APUTri(void) :
    APUOscillator::APUOscillator(),
    mReloadFlag(false),
    mLinearReload(0),
    mLinearCounter(0),
    mAngle(0.0f)
{
    mRealFrequency = (CPU_CLOCK_SPEED / 2) / ((mFrequency + 1) << 4);
    mOffset = mRealFrequency * NUM_OUTPUT_VALUES / mSampleRate;
}

void APUTri::setLinearCounter(const Byte& counterValue) {
    mLinearCounter  = counterValue;
    mLinearReload   = counterValue;
}

void APUTri::updateLength(void) {
    if (!mNoteLength) { return; }
    if (!mIsEnabled || mIsLooping) { return; }
    if (--mNoteLength != 0) {
        if(mLinearCounter)
            this->setAmplitude((Byte)MAX_AMPLITUDE);
        return;
    }
    this->setAmplitude(0);
}

void APUTri::updateLinearCounter(void) {
    if (!mLinearCounter) { return; }
    if (--mLinearCounter != 0) {
        if(mNoteLength || mIsLooping)
            this->setAmplitude((Byte)MAX_AMPLITUDE);
        return; 
    }
    if (mReloadFlag) 
        mLinearCounter = mLinearReload;
    if (!mIsLooping) 
        mReloadFlag = false;
    if (!mLinearCounter)
        this->setAmplitude(0);
}

void APUTri::setFrequency(const Byte& frequency, const bool& highByteWrite) {
    APUOscillator::setFrequency(frequency, highByteWrite);
    mRealFrequency = (CPU_CLOCK_SPEED / 2) / ((mFrequency + 1) << 4);
    mOffset = mRealFrequency * NUM_OUTPUT_VALUES / mSampleRate;
}

void APUTri::setSampleRate(const unsigned int& sampleRate) {
    APUOscillator::setSampleRate(sampleRate);
    mOffset = mRealFrequency * NUM_OUTPUT_VALUES / mSampleRate;
}

float APUTri::process(void) {
    if (!mIsEnabled) { return 0.0f; }
    mAngle += mOffset;
    mAngle -= mAngle >= 32.0f ? 32.0f : 0.0f;
    Byte idx = mAngle;
    return mRealAmplitude * OUTPUT_VALUES[idx & 0x1F] / MAX_OUTPUT_VALUE;
}


/*************/
/* APU PULSE */
/*************/


APUPulse::APUPulse(void) :
    APUOscillator::APUOscillator(),
    mIsSweeping(false),
    mSweepDown(false),
    mSweepPeriod(0),
    mSweepShift(0),
    mSweepClock(0),
    mDutyCycle(DEFAULT_DUTY_CYCLE),
    mAngle(0.0f)
{
    mRealFrequency = CPU_CLOCK_SPEED / ((mFrequency + 1) << 4);
    mOffset = mRealFrequency / mSampleRate;
}

void APUPulse::setDutyCycle(const float& dutyCycle) {
    if (dutyCycle < 0.0f || dutyCycle > 1.0f) { return; }
    mDutyCycle = dutyCycle;
}

void APUPulse::setFrequency(const Byte& frequency, const bool& highByteWrite) {
    APUOscillator::setFrequency(frequency, highByteWrite);
    mRealFrequency = CPU_CLOCK_SPEED / ((mFrequency + 1) << 4);
    mOffset = mRealFrequency / mSampleRate;
}

void APUPulse::setSampleRate(const unsigned int& sampleRate) {
    APUOscillator::setSampleRate(sampleRate);
    mOffset = mRealFrequency / mSampleRate;
}

void APUPulse::setSweepPeriod(const Byte& period) {
    mSweepPeriod    = period;
    mSweepClock     = period;
}

void APUPulse::updateSweep(void) {
    //the sweep unit mutes the oscillator, even when it's not enabled
    if (mFrequency < 8 || mFrequency > 0x7FF) {
        this->setAmplitude(0);
        mIsEnabled = false;
    }

    if (!mIsEnabled || !mIsSweeping || mSweepClock == 0) { return; }
    else if (--mSweepClock) { return; }

    mSweepClock = mNoteLength != 0 ? mSweepPeriod : 0;

    Byte freqChange = mFrequency >> mSweepShift;
    mFrequency = mSweepDown ? mFrequency - freqChange : mFrequency + freqChange;
    mRealFrequency = CPU_CLOCK_SPEED / ((mFrequency + 1) << 4);
    mOffset = mRealFrequency / mSampleRate;
}

float APUPulse::process(void) {
    if (!mIsEnabled) { return 0.0f; }
    mAngle += mOffset;
    mAngle -= mAngle > 1.0f ? 1.0f : 0.0f;
    return mAngle < mDutyCycle ? mRealAmplitude * -1.0f : mRealAmplitude;
}
