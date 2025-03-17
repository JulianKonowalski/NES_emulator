#include "NES/APU/APU.h"

using Byte = APU::Byte;
using Word = APU::Word;

/* APU OSCILLATORS */

APUPulse::APUPulse(void) :
    PulseOscillator::PulseOscillator(),
    mIsEnabled(false),
    mIsLooping(false),
    mIsSweeping(false),
    mSweepDown(false),
    mHasConstantVolume(false),
    mDivider(0),
    mNoteLength(0),
    mSweepPeriod(0),
    mSweepShift(0),
    mSweepClock(0),
    mInitialFreqency(0),
    mInitialAmplitude(0),
    mCurrentAmplitude(0)
{}

APUPulse::APUPulse(const unsigned int& sampleRate) :
    PulseOscillator::PulseOscillator(sampleRate),
    mIsEnabled(false),
    mIsLooping(false),
    mIsSweeping(false),
    mSweepDown(false),
    mHasConstantVolume(false),
    mDivider(0),
    mNoteLength(0),
    mSweepPeriod(0),
    mSweepShift(0),
    mSweepClock(0),
    mInitialFreqency(0),
    mInitialAmplitude(0),
    mCurrentAmplitude(0)
{}

void APUPulse::setVolumeSettings(const Byte& settings) {
    mIsLooping = settings & VOL_MASK::LOOPING;
    mInitialAmplitude = settings & VOL_MASK::VOLUME;
    mHasConstantVolume = settings & VOL_MASK::CONSTVOL;
    mCurrentAmplitude = mHasConstantVolume ? mInitialAmplitude : (Byte)MAX_AMPLITUDE;
    mDivider = mInitialAmplitude;
    PulseOscillator::setAmplitude(mCurrentAmplitude / MAX_AMPLITUDE);
}

void APUPulse::setNoteLength(const Byte& length) {
    mNoteLength = length;
}

void APUPulse::setSweepSettings(const Byte& settings) {
    mIsSweeping = settings & SWEEP_MASK::ENABLE;
    if (!mIsSweeping) { return; }

    mSweepPeriod = (settings & SWEEP_MASK::PERIOD) >> 4;
    mSweepClock = mSweepPeriod;
    mSweepDown = settings & SWEEP_MASK::DIRECTION;
    mSweepShift = settings & SWEEP_MASK::SHIFT;
}

void APUPulse::setFrequency(const Byte& frequency, const bool& highByteWrite) {
    if (!highByteWrite) {
        mInitialFreqency = (mInitialFreqency & 0xFF00) | frequency;
    } else {
        mIsEnabled = true;
        mInitialFreqency = (mInitialFreqency & 0x00FF) | (frequency << 8);
        PulseOscillator::setFrequency(
            CPU_CLOCK_SPEED
            / ( (mInitialFreqency + 1) << 4 )
        );
    }
}   

void APUPulse::updateEnvelope(void) {

    if (!mIsEnabled || mHasConstantVolume) { return; } //no need to update
    if (--mDivider) { return; } //divider > 0

    mDivider = mInitialAmplitude + 1;
    if (mCurrentAmplitude != 0) {
        --mCurrentAmplitude;
    } else {
        mCurrentAmplitude = mIsLooping ? (Byte)MAX_AMPLITUDE : 0;
    }
    PulseOscillator::setAmplitude(mCurrentAmplitude / 15.0f);

}

void APUPulse::updateLength(void) {
    if (!mIsEnabled || mIsLooping)
        return;

    if (--mNoteLength == 0)
        mIsEnabled = false;
}

void APUPulse::updateSweep(void) {
    //the sweep unit mutes the oscillator, even when it's not enabled
    if (mInitialFreqency < 8 || mInitialFreqency > 0x7FF)
        mIsEnabled = false;

    if (!mIsEnabled || !mIsSweeping || mSweepClock == 0) { return; }
    else if (--mSweepClock) { return; }

    mSweepClock = mNoteLength != 0 ? mSweepPeriod : 0;

    Byte freqChange = mInitialFreqency >> mSweepShift;
    mInitialFreqency = mSweepDown ? mInitialFreqency - freqChange : mInitialFreqency + freqChange;
    PulseOscillator::setFrequency(
        CPU_CLOCK_SPEED
        / ( (mInitialFreqency + 1) << 4 )
    );
}

float APUPulse::process(void) {
    return mIsEnabled ? PulseOscillator::process() : 0.0f;
}

/* APU */

APU::APU(Window* window, const unsigned int& sampleRate) :
    mCycles(0),
    mAudioBufferSize(window->getAudioBufferSize())
{
    mAudioBuffer = new short[mAudioBufferSize];

    mPulseOscillator[0].setAmplitude(0.0);
    mPulseOscillator[1].setAmplitude(0.0);
    mTriOscillator.setAmplitude(0.0f);
    mNoiseOscillator.setAmplitude(0.0f);
    if (sampleRate != Oscillator::DEFAULT_SAMPLE_RATE) {
        mPulseOscillator[0].setSampleRate(sampleRate);
        mPulseOscillator[1].setSampleRate(sampleRate);
        mTriOscillator.setSampleRate(sampleRate);
        mNoiseOscillator.setSampleRate(sampleRate);
    }

    window->setAudioStreamCallback(
        [this](void* buffer, unsigned int frames) {
            this->update(buffer, frames);
        }
    );
    window->playAudioStream();
}

APU::~APU(void) {
    delete [] mAudioBuffer;
}

void APU::clock(void) {
    ++mCycles;
    switch (mCycles) { //these are predefined cycles and their behaviour
        case 3728:  //quarter frame
            for (int i = 0; i < 2; ++i) {
                mPulseOscillator[i].updateEnvelope();
            }
            break;
        case 7456:  //half frame
            for (int i = 0; i < 2; ++i) {
                mPulseOscillator[i].updateEnvelope();
                mPulseOscillator[i].updateLength();
                mPulseOscillator[i].updateSweep();
            }
            break;
        case 11185: //quarter frame
            for (int i = 0; i < 2; ++i) {
                mPulseOscillator[i].updateEnvelope();
            }
            break;
        case 14914: //half frame
            if (mMode >> 7) { return; } //if in 5 step mode, the rest is executed quarter frame later
            for (int i = 0; i < 2; ++i) {
                mPulseOscillator[i].updateEnvelope();
                mPulseOscillator[i].updateLength();
                mPulseOscillator[i].updateSweep();
            }
            mCycles = 0;
            break;
        case 18640: //this is reached only in 5 step mode
            for (int i = 0; i < 2; ++i) {
                mPulseOscillator[i].updateEnvelope();
                mPulseOscillator[i].updateLength();
                mPulseOscillator[i].updateSweep();
            }
            mCycles = 0;
            break;
        default: break;
    }
}

Byte APU::readRegister(const Word& address) { return 0; }

void APU::writeRegister(const Byte& data, const Word& address) {
    switch (address) {
        case SQ1_VOL:   this->writePulseVolume(data, 0);    break;
        case SQ1_SWEEP: this->writePulseSweep(data, 0);     break;
        case SQ1_LO:    this->writePulseLo(data, 0);        break;
        case SQ1_HI:    this->writePulseHi(data, 0);        break;
        case SQ2_VOL:   this->writePulseVolume(data, 1);    break;
        case SQ2_SWEEP: this->writePulseSweep(data, 1);     break;
        case SQ2_LO:    this->writePulseLo(data, 1);        break;
        case SQ2_HI:    this->writePulseHi(data, 1);        break;
        case TRI_LINEAR:
            break;
        case TRI_LO:
            break;
        case TRI_HI:
            break;
        case NOISE_VOL:
            break;
        case NOISE_LO:
            break;
        case NOISE_HI:
            break;
        case DMC_FREQ:
            break;
        case DMC_RAW:
            break;
        case DMC_START:
            break;
        case DMC_LEN:
            break;
        case MODE:      mMode = data;                       break;
        default:        break;
    }
}

void APU::update(void* buffer, unsigned int frames) {
    short* d = (short*)buffer;
    for (unsigned int i = 0; i < frames; ++i) {
        d[i] = (short)(32000.0f * this->getSample());
    }
}

void APU::writePulseVolume(const Byte& data, const Byte& oscIdx) {
    mPulseOscillator[oscIdx].setDutyCycle(mOscLUT.getDutyCycle((data & VOL_MASK::DUTY) >> 6));
    mPulseOscillator[oscIdx].setVolumeSettings(data);
}

void APU::writePulseSweep(const Byte& data, const Byte& oscIdx) {
    mPulseOscillator[oscIdx].setSweepSettings(data);
}

void APU::writePulseLo(const Byte& data, const Byte& oscIdx) {
    mPulseOscillator[oscIdx].setFrequency(data, false);
}

void APU::writePulseHi(const Byte& data, const Byte& oscIdx) {
    mPulseOscillator[oscIdx].setEnabled(true);
    mPulseOscillator[oscIdx].setNoteLength(mOscLUT.getNoteLength(data >> 3));
    mPulseOscillator[oscIdx].setFrequency(data & 0x07, true);
}

float APU::getSample(void) {
    float sample = 0;
    sample += mPulseOscillator[0].process();
    sample += mPulseOscillator[1].process();
    return sample / 2;
}