#include "NES/APU/APU.h"

#include "raylib.h"

APU::APU(Window* window, const unsigned int& sampleRate) :
    mAudioBufferSize(window->getAudioBufferSize())
{
    mAudioBuffer = new short[mAudioBufferSize];

    window->setAudioStreamCallback(
        [this](void* buffer, unsigned int frames) {
            this->update(buffer, frames);
        }
    );

    if (sampleRate != Oscillator::DEFAULT_SAMPLE_RATE) {
        mPulseOscillator[0].setSampleRate(sampleRate);
        mPulseOscillator[1].setSampleRate(sampleRate);
        mTriOscillator.setSampleRate(sampleRate);
        mNoiseOscillator.setSampleRate(sampleRate);
    }

    window->playAudioStream();
}

APU::~APU(void) {
    delete [] mAudioBuffer;
}

void APU::update(void* buffer, unsigned int frames) {
    short* d = (short*)buffer;
    for (unsigned int i = 0; i < frames; ++i) {
        d[i] = (short)(
            32000.0f * (
                (
                    //mTriOscillator.process() +
                    mPulseOscillator[0].process()
                    //mNoiseOscillator.process()
                )
            )
        );
    }
}