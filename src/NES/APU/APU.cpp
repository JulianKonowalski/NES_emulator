#include "NES/APU/APU.h"

using Byte = APU::Byte;
using Word = APU::Word;

APU::APU(Window* window, const unsigned int& sampleRate) :
    mCycles(0),
    mMode(0),
    mAudioBufferSize(window->getAudioBufferSize())
{
    mAudioBuffer = new short[mAudioBufferSize];

    if (sampleRate != APUOscillator::DEFAULT_SAMPLE_RATE) {
        mPulse[0].setSampleRate(sampleRate);
        mPulse[1].setSampleRate(sampleRate);
        mTriangle.setSampleRate(sampleRate);
        mNoise.setSampleRate(sampleRate);
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
            mPulse[0].updateVolume();
            mPulse[1].updateVolume();
            mTriangle.updateLinearCounter();
            mNoise.updateVolume();
            break;
        case 7456:  //half frame
            mPulse[0].updateVolume();
            mPulse[0].updateLength();
            mPulse[0].updateSweep();

            mPulse[1].updateVolume();
            mPulse[1].updateLength();
            mPulse[1].updateSweep();

            mTriangle.updateLinearCounter();
            mTriangle.updateLength();

            mNoise.updateVolume();
            mNoise.updateLength();
            break;
        case 11185: //quarter frame
            mPulse[0].updateVolume();

            mPulse[1].updateVolume();

            mTriangle.updateLinearCounter();

            mNoise.updateVolume();
            break;
        case 14914: //half frame
            if (mMode >> 7) { return; } //if in 5 step mode, the rest is executed quarter frame later
            mPulse[0].updateVolume();
            mPulse[0].updateLength();
            mPulse[0].updateSweep();

            mPulse[1].updateVolume();
            mPulse[1].updateLength();
            mPulse[1].updateSweep();

            mTriangle.updateLinearCounter();
            mTriangle.updateLength();

            mNoise.updateVolume();
            mNoise.updateLength();
            mCycles = 0;
            break;
        case 18640: //this is reached only in 5 step mode
            mPulse[0].updateVolume();
            mPulse[0].updateLength();
            mPulse[0].updateSweep();

            mPulse[1].updateVolume();
            mPulse[1].updateLength();
            mPulse[1].updateSweep();

            mTriangle.updateLinearCounter();
            mTriangle.updateLength();

            mNoise.updateVolume();
            mNoise.updateLength();
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
        case TRI_LINEAR:this->writeTriLinear(data);         break;
        case TRI_LO:    this->writeTriLo(data);             break;
        case TRI_HI:    this->writeTriHi(data);             break;
        case NOISE_VOL: this->writeNoiseVolume(data);       break;
        case NOISE_LO:  this->writeNoiseLo(data);           break;
        case NOISE_HI:  this->writeNoiseHi(data);           break;
        case DMC_FREQ:
            break;
        case DMC_RAW:
            break;
        case DMC_START:
            break;
        case DMC_LEN:
            break;
        case STATUS:    this->updateStatus(data);           break;
        case FRAME_COUNTER:     mMode = data;               break;
        default:                                            break;
    }
}

void APU::update(void* buffer, unsigned int frames) {
    short* d = (short*)buffer;
    for (unsigned int i = 0; i < frames; ++i) {
        d[i] = (short)(32000.0f * this->getSample());
    }
}

void APU::writePulseVolume(const Byte& data, const Byte& oscIdx) {
    Byte dutyCycleCode = (data & VOL_MASK::DUTY) >> 6;
    mPulse[oscIdx].setDutyCycle(mOscLUT.getDutyCycle(dutyCycleCode));
    mPulse[oscIdx].setLooping(data & VOL_MASK::LOOPING);
    mPulse[oscIdx].setConstantVolume(data & VOL_MASK::CONSTVOL);
    mPulse[oscIdx].setAmplitude(data & VOL_MASK::VOLUME);
}

void APU::writePulseSweep(const Byte& data, const Byte& oscIdx) {
    mPulse[oscIdx].setSweepEnabled(data & SWEEP_MASK::ENABLE);
    mPulse[oscIdx].setSweepPeriod((data & SWEEP_MASK::PERIOD) >> 4);
    mPulse[oscIdx].setSweepDown(data & SWEEP_MASK::DIRECTION);
    mPulse[oscIdx].setSweepShift(data & SWEEP_MASK::SHIFT);
}

void APU::writePulseLo(const Byte& data, const Byte& oscIdx) {
    mPulse[oscIdx].setFrequency(data, false);
}

void APU::writePulseHi(const Byte& data, const Byte& oscIdx) {
    mPulse[oscIdx].setNoteLength(mOscLUT.getNoteLength(data >> 3));
    mPulse[oscIdx].setFrequency(data & 0x7, true);
}

void APU::writeTriLinear(const Byte& data) {
    mTriangle.setLooping(data & (1 << 7));
    mTriangle.setLinearCounter(data & ~(1 << 7));
}

void APU::writeTriLo(const Byte& data) {
    mTriangle.setFrequency(data, false);
}

void APU::writeTriHi(const Byte& data) {
    mTriangle.setFrequency((data & 0x7), true);
    mTriangle.setNoteLength(mOscLUT.getNoteLength(data >> 3));
    mTriangle.setReloadFlag();
}

void APU::writeNoiseVolume(const Byte& data) {
    mNoise.setAmplitude(data & 0x0F);
    mNoise.setConstantVolume(data & (1 << 4));
    mNoise.setLooping(data & (1 << 5));
}

void APU::writeNoiseLo(const Byte& data) {
    mNoise.setModeFlag(data & (1 << 7));
    Word freq = mOscLUT.getNoiseFrequency(data & 0x0F);
    mNoise.setFrequency(freq, false);
    mNoise.setFrequency((freq >> 8) & 0x7, true);
}

void APU::writeNoiseHi(const Byte& data) {
    mNoise.setNoteLength(mOscLUT.getNoteLength(data >> 3));
}

void APU::updateStatus(const Byte& data) {
    mPulse[0].setEnabled(data & 1);
    mPulse[1].setEnabled(data & (1 << 1));
    mTriangle.setEnabled(data & (1 << 2));
    mNoise.setEnabled(data & (1 << 3));
}

float APU::getSample(void) {
    float sample = 0;
    sample += mPulse[0].process();
    sample += mPulse[1].process();
    sample += mTriangle.process();
    sample += mNoise.process();
    return sample / 4;
}