#include "NES/APU/APU.h"

#include "raylib.h"

APU::APU(Window* window, const unsigned int& sampleRate) :
    mOscillator(sampleRate),
    mTriOscillator(sampleRate),
    mNoiseOscillator(sampleRate),
    mAudioBufferSize(window->getAudioBufferSize()),
    mAudioBuffer(new short[mAudioBufferSize])
{
    window->setAudioStreamCallback(
        [this](void* buffer, unsigned int frames) {
            this->update(buffer, frames);
        }
    );

    mOscillator.setFrequency(440.0f);
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
                    //mOscillator.process() +
                    mTriOscillator.process()
                )
                /// 2
            )
        );
    }
}