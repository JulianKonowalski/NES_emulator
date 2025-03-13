#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cstdint>

class Oscillator {
public:
	Oscillator(const unsigned int& sampleRate);
    virtual float process(void);
protected:
	const unsigned int mSampleRate; //just in case someone wants more than 44,1kHz
};


class NoiseOscillator : public Oscillator {
public:
	using Oscillator::Oscillator;
	float process(void);
};


class SinOscillator : public Oscillator {
public:

	SinOscillator(const unsigned int& sampleRate);
	SinOscillator(const float& frequency, const unsigned int& sampleRate);

	void setFrequency(const float& frequency);
	float getFrequency(void) { return mFrequency; }

	float process(void);

protected:

	float mFrequency;
	float mAngle;
	float mOffset;

};


class TriOscillator : public SinOscillator {
public:
	using SinOscillator::SinOscillator;
	float process(void);
};


class PulseOscillator : public SinOscillator {
public:

	PulseOscillator(const unsigned int& sampleRate);
	PulseOscillator(const float& frequency, const unsigned int& sampleRate);

	void setPhaseShift(const short& phaseShift);
	uint16_t getPhaseShift(void);

	float process(void);

private:

	uint8_t mHarmonics;			//number of harmonics to construct the wave
	short mPhaseShiftDeg;		//phase shift in degrees
	float mPhaseShiftRad;		//phase shift in radians
	float mVolNorm;				//divisor of the final sample to normalise the volume

};

#endif