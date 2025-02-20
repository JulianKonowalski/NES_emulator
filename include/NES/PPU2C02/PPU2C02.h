#ifndef PPU_H
#define PPU_H

#include <cstdint>

#include "IO/Screen.h"
#include "NES/Buses/PPUBus.h"
#include "NES/PPU2C02/ColourLUT.h"

enum PPU_REGISTER {
    PPUCTRL,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROLL,
    PPUADDR,
    PPUDATA,
    OAMDMA
};

class PPU2C02 {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    PPU2C02(void);

    void boot(PPUBus& bus, Screen& screen);

    void clock(void);

    Byte readRegister(const Word& address);
    void writeRegister(const Byte& data, const Word& address);

private:

    PPUBus* mBus;
    Screen* mScreen;

    const ColourLUT mColours;

    Byte mRegisters[8];

    Byte mPpuDataBuffer;
    Word mPpuAddr;
    bool mAddrLatch;

    short mRow;
    short mColumn;
};

#endif // !PPU_H
