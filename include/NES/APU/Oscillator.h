#ifndef OSCILLATOR_H
#define OSCILLATOR_H


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



//
//
//class  PulseOscillator : public Oscillator {
//public:
//	using Oscillator::Oscillator;
//
//	PulseOscillator(const float& frequency, const uint8_t& harmonics, const float& phaseShift, const float& angle, const unsigned int& sampleRate);
//	PulseOscillator(const unsigned int& sampleRate);
//
//	void setHarmonics(const uint8_t& harmonics);
//	void setPhaseShift(const float& phaseShift);
//
//	float process(void);
//
//private:
//
//	uint8_t mHarmonics;
//	float mPhaseShift;
//	float mVolNorm;
//
//};


#endif