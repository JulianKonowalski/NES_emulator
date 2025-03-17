#ifndef OSC_LUT_H
#define OSC_LUT_H

#include <cstdint>

class OscLUT {

    friend class APU;   //only APU can create this LUT

    using Byte = uint8_t;

    const float& getDutyCycle(const Byte& code) const { return mDutyCycles[code & 0x3]; }
    const Byte& getNoteLength(const Byte& code) const { return mNoteLengths[code & 0x1F]; }

    const float mDutyCycles[4] = {
        0.125f,
        0.250f,
        0.500f,
        0.750f
    };

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
};

#endif // !OSC_LUT_H
