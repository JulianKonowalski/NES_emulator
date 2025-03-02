#ifndef CPUBUS_H
#define CPUBUS_H

#include <cstdint>

#include "NES/PPU2C02/PPU2C02.h"
#include "NES/Cartridge/Cartridge.h"

#include "IO/Joypad.h"

class CPUBus {
public:
    using Byte = uint8_t;
    using Word = uint16_t;

    CPUBus(PPU2C02& ppu, Cartridge& cartridge, Joypad& joypad);

    Byte read(const Word& address);
    void write(const Byte& data, const Word& address);

private:
    Byte mRam[2048];
    PPU2C02* mPpu;
    Cartridge* mCartridge;
    Joypad* mJoypad;
};

#endif // !CPUBUS_H
