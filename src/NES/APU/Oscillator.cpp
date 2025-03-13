#include "NES/APU/Oscillator.h"

#include <cstdlib>

#define PI	3.1415926535f
#define PI2 6.2831853071f

/**
* shoutout to my friend, ezic04,
* who helped me out with this 
* fast implementation of sin.
* Well tbh he just wrote it 
* for me.
*/

static constexpr float T1 = 0.0778267541633225f;

float sin2pi(float x) {
  float arg = x - (long)x;
  bool sign = arg >= 0.5f;
  arg = arg >= 0.5f ? 1.f - arg : arg;
  //////////////// this part
  arg = arg > 0.25f ? 0.5f - arg : arg;

  if (arg <= T1) {
    float temp = PI2 * arg;
    float val = temp * (1 - temp * temp / 6);
    return sign ? -val : val;
  }
  ////////////// couild be commented
  float temp = 8.f * arg * (1.f - 2.f * arg);
  float val = 4.f * temp / (5.f - temp);
  return sign ? -val : val;
}




/* BASE OSCILLATOR */

Oscillator::Oscillator(const unsigned int& sampleRate) :
    mSampleRate(sampleRate)
{}

float Oscillator::process(void) {
    return 0.0f;
}




/* NOISE OSCILLATOR */

float NoiseOscillator::process(void) {
    return (float)rand() / RAND_MAX;
}




/* SINE OSCILLATOR */

SinOscillator::SinOscillator(const unsigned int& sampleRate) :
    Oscillator::Oscillator(sampleRate),
    mFrequency(0),
    mAngle(0),
    mOffset(0)
{}

SinOscillator::SinOscillator(const float& frequency, const unsigned int& sampleRate) :
    Oscillator::Oscillator(sampleRate),
    mFrequency(0),
    mAngle(0)
{
    mOffset = mFrequency / mSampleRate;
}

void SinOscillator::setFrequency(const float& frequency) {
    if (frequency < 20.0f || frequency > 20000.0f) { return; }
    mFrequency = frequency;
    mOffset = mFrequency / mSampleRate;
}

float SinOscillator::process(void) {
    mAngle += mOffset;
    mAngle = mAngle > 1.0f ? mAngle - 1.0f : mAngle;
    return sin2pi(mAngle);
}




/* TRIANGLE OSCILLATOR */

float TriOscillator::process(void) {
    mAngle += mOffset;
    mAngle = mAngle > 2.0f ? mAngle - 2.0f : mAngle;
    return mAngle - 1.0f;
}


//
//
//
//
//PulseOscillator::PulseOscillator(const float& frequency, const uint8_t& harmonics, const float& phaseShift, const float& angle, const unsigned int& sampleRate) : 
//    Oscillator(frequency, angle, sampleRate),
//    mPhaseShift(phaseShift)
//{
//    this->setHarmonics(harmonics);
//}
//
//PulseOscillator::PulseOscillator(const unsigned int& sampleRate) :
//    Oscillator(sampleRate),
//    mHarmonics(10),
//    mPhaseShift(0.5f),
//    mVolNorm(10)
//{}
//
//void PulseOscillator::setHarmonics(const uint8_t& harmonics) {
//    mHarmonics = harmonics;
//    mVolNorm = 0;
//    for (int i = 0; i < mHarmonics; ++i)
//        mVolNorm += 1.0f / i;
//}
//
//void PulseOscillator::setPhaseShift(const float& phaseShift) {
//    mPhaseShift = PI2 * phaseShift;
//}
//
//float PulseOscillator::process(void) {
//    //float baseSample = 0;
//    //float offsetSample = 0;
//    //for (int i = 1; i <= mHarmonics; ++i) {
//    //	baseSample += std::sinf(i * mAngle) / i;
//    //	offsetSample += std::cosf(i * (mAngle - mPhaseShift)) / i;
//    //}
//    //mAngle += mOffset;
//    //return (baseSample - offsetSample) / mVolNorm;
//
//    mAngle += mOffset;
//    return std::sinf(mAngle);
//}
