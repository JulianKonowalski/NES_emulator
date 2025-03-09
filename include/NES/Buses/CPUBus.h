#ifndef CPUBUS_H
#define CPUBUS_H

#include <cstdint>

#include "NES/PPU2C02/PPU2C02.h"
#include "NES/Cartridge/Cartridge.h"

#include "IO/Joypad.h"

class MOS6502;

class CPUBus {
public:
    using Byte = uint8_t;
    using Word = uint16_t;

    CPUBus(MOS6502& cpu, PPU2C02& ppu, Cartridge& cartridge, Joypad& joypad, Word& globalClock);

    Byte read(const Word& address);
    void write(const Byte& data, const Word& address);

    void dmaTransfer(void);

private:
    Byte mRam[2048];

    MOS6502* mCpu;
    PPU2C02* mPpu;
    Cartridge* mCartridge;
    Joypad* mJoypad;

    bool mDmaWait;
    Byte mDmaData;
    Word* mGlobalClock;
};

#endif // !CPUBUS_H
