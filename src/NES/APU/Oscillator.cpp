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
    mAmplitude(Oscillator::DEFAULT_AMPLITUDE),
    mSampleRate(sampleRate)
{}

Oscillator::Oscillator(void) :
    mAmplitude(Oscillator::DEFAULT_AMPLITUDE),
    mSampleRate(Oscillator::DEFAULT_SAMPLE_RATE)
{}

void Oscillator::setAmplitude(const float& amplitude) {
    if (amplitude < 0.0f || amplitude > 1.0f) { return; }
    mAmplitude = amplitude;
} 

float Oscillator::process(void) {
    return 0.0f;
}




/* NOISE OSCILLATOR */

float NoiseOscillator::process(void) {
    return mAmplitude * (((float)rand() / (RAND_MAX >> 1)) - 1.0f);
}




/* PITCHED OSCILLATOR */

PitchedOscillator::PitchedOscillator(void) :
    Oscillator::Oscillator(),
    mAngle(0)
{
    this->setFrequency(PitchedOscillator::DEFAULT_FREQUENCY);
}

PitchedOscillator::PitchedOscillator(const unsigned int& sampleRate) :
    Oscillator::Oscillator(sampleRate),
    mAngle(0)
{
    this->setFrequency(PitchedOscillator::DEFAULT_FREQUENCY);
}

PitchedOscillator::PitchedOscillator(const float& frequency, const unsigned int& sampleRate) :
    Oscillator::Oscillator(sampleRate),
    mAngle(0)
{
    if (frequency < 20.0f || frequency > 20000.0f)
        this->setFrequency(PitchedOscillator::DEFAULT_FREQUENCY);
    else
        this->setFrequency(frequency);
}

void PitchedOscillator::setFrequency(const float& frequency) {
    if (frequency < 20.0f || frequency > 20000.0f) { return; }
    mFrequency = frequency;
    mOffset = mFrequency / mSampleRate;
}




/* SINE OSCILLATOR */

float SinOscillator::process(void) {
    mAngle += mOffset;
    mAngle -= mAngle > 1.0f ? 1.0f : 0;
    return mAmplitude * sin2pi(mAngle);
}




/* TRIANGLE OSCILLATOR */

float TriOscillator::process(void) {
    mAngle += mOffset;
    mAngle -= mAngle > 2.0f ? 2.0f : 0;
    return mAmplitude * (mAngle - 1.0f);
}




/* PULSE WAVE */

PulseOscillator::PulseOscillator(void) :
    PitchedOscillator::PitchedOscillator()
{
    this->setDutyCycle(PulseOscillator::DEFAULT_DUTY_CYCLE);
}

PulseOscillator::PulseOscillator(const unsigned int& sampleRate) :
    PitchedOscillator::PitchedOscillator(sampleRate)
{
    this->setDutyCycle(PulseOscillator::DEFAULT_DUTY_CYCLE);
}

PulseOscillator::PulseOscillator(const float& frequency, const unsigned int& sampleRate) :
    PitchedOscillator::PitchedOscillator(frequency, sampleRate)
{
    this->setDutyCycle(PulseOscillator::DEFAULT_DUTY_CYCLE);
}

void PulseOscillator::setDutyCycle(const float& dutyCycle) {
    if (dutyCycle < 0.0f || dutyCycle > 1.0f) { return; }
    mDutyCycle = dutyCycle;
}

float PulseOscillator::process(void) {
    mAngle += mOffset;
    mAngle -= mAngle > 1.0f ? 1.0f : 0;
    return mAngle < mDutyCycle ? mAmplitude * -1.0f : mAmplitude;
}