#ifndef OSC_LUT_H
#define OSC_LUT_H

#include <cstdint>

/**
* Lookup table for the 
* pulse oscillator's
* duty cycles, note lengths
* and noise oscillator's
* frequencies.
* 
* @see APU
* @see APUNoise
* @see APUPulse
*/
class OscLUT {

    friend class APU;   //only APU can create this LUT

    using Byte = uint8_t;
    using Word = uint16_t;
    
    /**
    * Returns the duty cycle
    * for the given code.
    * 
    * @param code duty cycle
    *   code
    * 
    * @return fetched duty cycle
    * 
    * @see mDutyCycles
    */
    const float& getDutyCycle(const Byte& code) const { return mDutyCycles[code & 0x3]; }

    /**
    * Returns the note length
    * for the given code.
    * 
    * @param code note length
    *   code
    * 
    * @return fetched note length
    * 
    * @see mNoteLengths
    */
    const Byte& getNoteLength(const Byte& code) const { return mNoteLengths[code & 0x1F]; }

    /**
    * Returns the nose frequency
    * for the given code.
    * 
    * @param code noise frequency
    *   code
    * 
    * @return fetched noise frequency
    * 
    * @see mNoiseFrequency
    */
    const Word& getNoiseFrequency(const Byte& code) const { return mNoiseFrequency[code & 0xF]; }

    /** An array of available duty cycles */
    const float mDutyCycles[4] = {
        0.125f,
        0.250f,
        0.500f,
        0.750f
    };

    /** An array of available note lengths */
    const Byte mNoteLengths[32] = {
        Byte(10),
        Byte(254),
        Byte(20),
        Byte(2),
        Byte(40),
        Byte(4),
        Byte(80),
        Byte(6),
        Byte(160),
        Byte(8),
        Byte(60),
        Byte(10),
        Byte(14),
        Byte(12),
        Byte(26),
        Byte(14),
        Byte(12),
        Byte(16),
        Byte(24),
        Byte(18),
        Byte(48),
        Byte(20),
        Byte(96),
        Byte(22),
        Byte(192),
        Byte(24),
        Byte(72),
        Byte(26),
        Byte(16),
        Byte(28),
        Byte(32),
        Byte(30)
    };

    /** An array of available noise frequencies */
    const Word mNoiseFrequency[16] = {
        Word(4),
        Word(8),
        Word(16),
        Word(32),
        Word(64),
        Word(96),
        Word(128),
        Word(160),
        Word(202),
        Word(254),
        Word(380),
        Word(508),
        Word(762),
        Word(1016),
        Word(2034),
        Word(4068)
    };
};

#endif // !OSC_LUT_H
