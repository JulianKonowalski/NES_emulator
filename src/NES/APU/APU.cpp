#include "NES/APU/APU.h"

#include "raylib.h"

using Byte = APU::Byte;
using Word = APU::Word;

APU::APU(Window* window, const unsigned int& sampleRate) :
    mAudioBufferSize(window->getAudioBufferSize())
{
    mAudioBuffer = new short[mAudioBufferSize];


    memset(mPulseFrequency, 0, 2);
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

Byte APU::readRegister(const Word& address) {
    return 0;
}

void APU::writeRegister(const Byte& data, const Word& address) {
    switch (address) {
    case SQ1_VOL:

        switch ((data & 0xC0) >> 6) {
            case 0x00: mPulseOscillator[0].setPhaseShift(0.125f * 180); break;
            case 0x01: mPulseOscillator[0].setPhaseShift(0.25f * 180); break;
            case 0x02: mPulseOscillator[0].setPhaseShift(0.50f * 180); break;
            case 0x03: mPulseOscillator[0].setPhaseShift(0.75f * 180); break;
            default: break;
        }
        break;

    case SQ1_SWEEP:
        break;
    case SQ1_LO:
        mPulseFrequency[0] = (mPulseFrequency[0] & 0xFF00) | data;
        break;
    case SQ1_HI:
        mPulseFrequency[0] = (mPulseFrequency[0] & 0x00FF) | ((data & 0x07) << 8);
        mPulseOscillator[0].setFrequency(
            1789773.0f / (16.0f * (mPulseFrequency[0] + 1))
        );
        break;
    case SQ2_VOL:
        break;
    case SQ2_SWEEP:
        break;
    case SQ2_LO:
        break;
    case SQ2_HI:
        break;
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
    default:
        break;
    }
}

void APU::update(void* buffer, unsigned int frames) {
    short* d = (short*)buffer;
    for (unsigned int i = 0; i < frames; ++i) {
        float sample = mPulseOscillator[0].process();
        d[i] = (short)(32000.0f * sample) / 8; //delete this /8
    }
}