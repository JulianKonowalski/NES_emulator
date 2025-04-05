#ifndef APU_H
#define APU_H

#include <cstdint>

#include "NES/APU/Oscillator.h"
#include "NES/APU/OscLUT.h"
#include "IO/Window.h"

/**
* Masks for extracting
* the data from SQ1_VOL
* and SQ2_VOL writes.
* 
* @see APU_REGISTER
*/
enum VOL_MASK {
    DUTY =      0b11000000,
    LOOPING =   0b00100000,
    CONSTVOL =  0b00010000,
    VOLUME =    0b00001111
};

/**
* Masks for extracting
* the data from SQ1_SWEEP
* and SQ2_SWEEP writes.
* 
* @see APU_REGISTER
*/
enum SWEEP_MASK {
    ENABLE =    0b10000000,
    PERIOD =    0b01110000,
    DIRECTION = 0b00001000,
    SHIFT =     0b00000111
};

/**
* Addresses of the
* APU registers.
*/
enum APU_REGISTER {
    SQ1_VOL =       0x4000,
    SQ1_SWEEP =     0x4001,
    SQ1_LO =        0x4002,
    SQ1_HI =        0x4003,
    SQ2_VOL =       0x4004,
    SQ2_SWEEP =     0x4005,
    SQ2_LO =        0x4006,
    SQ2_HI =        0x4007,
    TRI_LINEAR =    0x4008,
    TRI_LO =        0x400A,
    TRI_HI =        0x400B,
    NOISE_VOL =     0x400C,
    NOISE_LO =      0x400E,
    NOISE_HI =      0x400F,
    DMC_FREQ =      0x4010,
    DMC_RAW =       0x4011,
    DMC_START =     0x4012,
    DMC_LEN =       0x4013,
    STATUS =        0x4015,
    FRAME_COUNTER = 0x4017
};

/**
* Class emulating the
* behaviour of the NES'
* APU module.
*/
class APU {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    /** 
    * Class constructor. Initializes an instance
    * of the class with given parameters.
    * 
    * @param window application window that
    *   will handle audio output.
    * @param sampleRate audio device sample rate
    * 
    * @see Window
    */
    APU(Window* window, const unsigned int& sampleRate);

    /**
    * Class destructor. Dealocates the audio buffer.
    * 
    * @see mAudioBuffer
    */
    ~APU(void);

    /**
    * Clocks the APU. 
    */
    void clock(void);

    /**
    * Responds to a read call.
    * In this implementation the
    * actual read doesn't happen
    * and the method returns a dummy
    * value.
    * 
    * @param address address of the
    *   register to read.
    * 
    * @return dummy value (0)
    */
    Byte readRegister(const Word& address);

    /**
    * Writes data to one of the
    * APU registers. The action
    * is taken immediately upon the
    * write and the data is not stored
    * after it is finished.
    * 
    * @param data data to be written
    *   into the register
    * @param address the register's
    *   address
    */
    void writeRegister(const Byte& data, const Word& address);

    /**
    * Fills the audio buffer with
    * a given amount of samples.
    * Data written to a buffer is
    * scaled from floats ranging
    * from -1.0f - 1.0f to shorts
    * filling the whole short int 
    * value range.
    * 
    * @param buffer audio buffer to
    *   be filled
    * @param frames audio buffer length
    */
    void update(void* buffer, unsigned int frames);

private:

    /**
    * Writes volume data to
    * one of the internal
    * pulse wave oscillators.
    * 
    * @param data data to be 
    *   written to the oscillator
    * @param oscIdx index telling 
    *   the method which oscillator
    *   to write into
    * 
    * @see mPulse
    */
    void writePulseVolume(const Byte& data, const Byte& oscIdx);

    /**
    * Writes sweep data to
    * one of the internal
    * pulse wave oscillators.
    * 
    * @param data data to be 
    *   written to the oscillator
    * @param oscIdx index telling
    *   the method which oscillator
    *   to write into
    * 
    * @see mPulse
    */
    void writePulseSweep(const Byte& data, const Byte& oscIdx);

    /**
    * Writes the low byte of
    * period data to one of the
    * internal pulse wave oscillators.
    * 
    * @param data data to be written
    *   to the oscillator
    * @param oscIdx index telling
    *   the method which oscillator
    *   to write into
    * 
    * @see mPulse
    */
    void writePulseLo(const Byte& data, const Byte& oscIdx);

    /**
    * Writes the high byte of
    * period data to one of the
    * internal pulse wave oscillators.
    * 
    * @param data data to be written
    *   to the oscillator
    * @param oscIdx index telling 
    *   the method which oscillator
    *   to write into
    * 
    * @see mPulse
    */
    void writePulseHi(const Byte& data, const Byte& oscIdx);

    /**
    * Writes the linear counter data
    * to the internal triangle oscillator.
    * 
    * @param data data to be written
    *   to the oscillator
    * 
    * @see mTriangle
    * 
    */
    void writeTriLinear(const Byte& data);

    /**
    * Writes the low byte of
    * period data to the internal
    * triangle oscillator.
    * 
    * @param data data to be written
    *   to the oscillator
    * 
    * @see mTriangle
    */
    void writeTriLo(const Byte& data);

    /**
    * Writes the high byte of
    * period data to the internal
    * triange oscillator.
    * 
    * @param data data to be written
    *   to the oscillator
    * 
    * @see mTriangle
    */
    void writeTriHi(const Byte& data);

    /**
    * Writes the volume data to
    * the internal noise oscillator
    * 
    * @param data data to be written
    *   to the oscillator
    * 
    * @see mNoise
    */
    void writeNoiseVolume(const Byte& data);

    /**
    * Writes the low byte of 
    * period data to the internal
    * noise oscillator.
    * 
    * @param data data to be written
    *   to the oscillator
    * 
    * @see mNoise
    */
    void writeNoiseLo(const Byte& data);

    /**
    * Writes the high byte of
    * period data to the internal
    * noise oscillator.
    * 
    * @param data data to be written
    *   to the oscillator
    * 
    * @see mNoise
    */
    void writeNoiseHi(const Byte& data);

    /**
    * Turns the oscillators on or off
    * depending on the supplied data.
    * Each oscillator can be manipulated
    * independently.
    * 
    * @param data new status of the APU
    */
    void updateStatus(const Byte& data);

    /**
    * Clocks all oscillators
    * and mixes their signals
    * to get the final sample.
    * 
    * @return final sample
    * 
    * @see mPulse
    * @see mTriangle
    * @see mNoise
    */
    float getSample(void);

    /**
    * Lookup table for fetching
    * pulse duty cycles, note
    * lengths and noise frequencies.
    * 
    * @see mPulse
    * @see mNoise
    */
    OscLUT mOscLUT;

    /**
    * Operating mode. The APU
    * has 2 modes, which dictate
    * the timing of the APU actions.
    */
    Byte mMode;

    /** Internal pulse oscillators */
    APUPulse mPulse[2];

    /** Internal triangle oscillator */
    APUTri mTriangle;

    /** Internal noise oscillator */
    APUNoise mNoise;

    /** Cycle counter */
    unsigned short mCycles;

    /** Audio buffer */
    short* mAudioBuffer;

    /** Audio buffer size */
    unsigned short mAudioBufferSize;

};

#endif // !APU
