#ifndef OSCILLATOR_H
#define OSCILLATOR_H

class Oscillator {
public:

	Oscillator(const unsigned int& sampleRate);
	Oscillator(void);

	void setAmplitude(const float& amplitude);
	float getAmplitude(void) { return mAmplitude; }

	void setSampleRate(const unsigned int& sampleRate) { mSampleRate = sampleRate; }
	int getSampleRate(void) { return mSampleRate; }

    float process(void);

	inline static constexpr float DEFAULT_AMPLITUDE = 1.0f;
	inline static constexpr unsigned int DEFAULT_SAMPLE_RATE = 44100;

protected:

	float mAmplitude;			//volume of the oscillator, 0.0 - 1.0
	unsigned int mSampleRate;	//just in case someone wants more than 44,1kHz

};


class NoiseOscillator : public Oscillator {
public:
	using Oscillator::Oscillator;
	float process(void);
};


class PitchedOscillator : public Oscillator {
public:

	PitchedOscillator(void);
	PitchedOscillator(const unsigned int& sampleRate);
	PitchedOscillator(const float& frequency, const unsigned int& sampleRate);

	void setFrequency(const float& frequency);
	float getFrequency(void) { return mFrequency; }

	inline static constexpr float DEFAULT_FREQUENCY = 440.0f;

protected:

	float mFrequency;
	float mAngle;
	float mOffset;

};


class SinOscillator : public PitchedOscillator {
public:
	using PitchedOscillator::PitchedOscillator;
	float process(void);
};


class TriOscillator : public PitchedOscillator {
public:
	using PitchedOscillator::PitchedOscillator;
	float process(void);
};


class PulseOscillator : public PitchedOscillator {
public:

	PulseOscillator(void);
	PulseOscillator(const unsigned int& sampleRate);
	PulseOscillator(const float& frequency, const unsigned int& sampleRate);

	void setDutyCycle(const float& dutyCycle);
	float getDutyCycle(void) { return mDutyCycle; }

	float process(void);

	inline static constexpr float DEFAULT_DUTY_CYCLE = 0.5f;
	
protected:

	float mDutyCycle;	//ratio between low and high signal

};

#endif