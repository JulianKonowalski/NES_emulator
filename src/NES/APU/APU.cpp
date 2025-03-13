#include "NES/APU/APU.h"

#include "raylib.h"

APU::APU(Window* window, const unsigned int& sampleRate) :
    mPulseOscillator1(sampleRate),
    mPulseOscillator2(sampleRate),
    mTriOscillator(sampleRate),
    mNoiseOscillator(sampleRate),
    mAudioBufferSize(window->getAudioBufferSize())
    //mAudioBuffer(new short[mAudioBufferSize])
{
    mAudioBuffer = new short[mAudioBufferSize];

    window->setAudioStreamCallback(
        [this](void* buffer, unsigned int frames) {
            this->update(buffer, frames);
        }
    );

    mPulseOscillator1.setFrequency(440.0f);
    mPulseOscillator2.setFrequency(440.0f);
    mTriOscillator.setFrequency(440.0f);

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
                    mTriOscillator.process() +
                    mPulseOscillator1.process() + 
                    mNoiseOscillator.process()
                )
                / 3
            )
        );
    }
}