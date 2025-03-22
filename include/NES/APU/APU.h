#ifndef APU_H
#define APU_H

#include <cstdint>

#include "NES/APU/Oscillator.h"
#include "NES/APU/OscLUT.h"
#include "IO/Window.h"

enum VOL_MASK {
    DUTY =      0b11000000,
    LOOPING =   0b00100000,
    CONSTVOL =  0b00010000,
    VOLUME =    0b00001111
};

enum SWEEP_MASK {
    ENABLE =    0b10000000,
    PERIOD =    0b01110000,
    DIRECTION = 0b00001000,
    SHIFT =     0b00000111
};

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

class APU {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    APU(Window* window, const unsigned int& sampleRate);
    ~APU(void);

    void clock(void);

    Byte readRegister(const Word& address);
    void writeRegister(const Byte& data, const Word& address);
    void update(void* buffer, unsigned int frames);

private:

    void writePulseVolume(const Byte& data, const Byte& oscIdx);
    void writePulseSweep(const Byte& data, const Byte& oscIdx);
    void writePulseLo(const Byte& data, const Byte& oscIdx);
    void writePulseHi(const Byte& data, const Byte& oscIdx);

    void writeTriLinear(const Byte& data);
    void writeTriLo(const Byte& data);
    void writeTriHi(const Byte& data);

    void writeNoiseVolume(const Byte& data);
    void writeNoiseLo(const Byte& data);
    void writeNoiseHi(const Byte& data);

    void updateStatus(const Byte& data);

    float getSample(void);

    OscLUT mOscLUT;
    Byte mMode;

    APUPulse mPulse[2];
    APUTri mTriangle;
    APUNoise mNoise;

    unsigned short mCycles;

    short* mAudioBuffer;
    unsigned short mAudioBufferSize;

};

#endif // !APU
