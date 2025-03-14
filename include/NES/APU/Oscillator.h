#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cstdint>

class Oscillator {
public:
	Oscillator(const unsigned int& sampleRate);
	Oscillator(void);
    virtual float process(void);

	void setSampleRate(const unsigned int& sampleRate) { mSampleRate = sampleRate; }
	int getSampleRate(void) { return mSampleRate; }

	inline static const unsigned int DEFAULT_SAMPLE_RATE = 44100;

protected:
	unsigned int mSampleRate; //just in case someone wants more than 44,1kHz
};


class NoiseOscillator : public Oscillator {
public:
	using Oscillator::Oscillator;
	float process(void);
};


class SinOscillator : public Oscillator {
public:

	SinOscillator(void);
	SinOscillator(const unsigned int& sampleRate);
	SinOscillator(const float& frequency, const unsigned int& sampleRate);

	void setFrequency(const float& frequency);
	float getFrequency(void) { return mFrequency; }

	float process(void);

	inline static const float DEFAULT_FREQUENCY = 440.0f;

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

	PulseOscillator(void);
	PulseOscillator(const unsigned int& sampleRate);
	PulseOscillator(const float& frequency, const unsigned int& sampleRate);

	void setHarmonics(const uint8_t& harmonics);
	uint8_t getHarmonics(void) { return mHarmonics; }
	void setPhaseShift(const short& phaseShift);
	uint16_t getPhaseShift(void) { return mPhaseShiftDeg; }

	float process(void);

	inline static const uint8_t DEFAULT_HARMONICS = 10;
	inline static const short DEFAULT_PHASE_SHIFT = 90;

private:

	uint8_t mHarmonics;			//number of harmonics to construct the wave
	short mPhaseShiftDeg;		//phase shift in degrees
	float mPhaseShiftRad;		//phase shift in radians
	float mVolNorm;				//divisor of the final sample to normalise the volume

};

#endif