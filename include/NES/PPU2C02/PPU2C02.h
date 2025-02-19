#ifndef PPU_H
#define PPU_H

#include <cstdint>

#include "IO/Screen.h"
#include "NES/Buses/PPUBus.h"

enum PPU_REGISTER {
    PPUCTRL,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROLL,
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

    Byte readRegister(const Word& address) { return mRegisters[address & 0x07]; }
    void writeRegister(const Byte& data, const Word& address) { mRegisters[address & 0x07] = data; }

private:
    PPUBus* mBus;
    Screen* mScreen;

    Byte mRegisters[8];
};

#endif // !PPU_H
