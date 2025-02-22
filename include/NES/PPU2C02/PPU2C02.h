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

enum CTRL_REGISTER {
    NTADDR1 = 1,        //LSB of base nametable address
    NTADDR2 = 1 << 1,   //MSB of base nametable address
    VRAMINC = 1 << 2,   //address increment per CPU read/write of PPUDATA
    SPRADDR = 1 << 3,   //sprite pattern table address
    BPTADDR = 1 << 4,   //background pattern table address
    SPRTSIZ = 1 << 5,   //sprite size
    PPUMSTR = 1 << 6,   //PPU master/slave select
    VBNMIEN = 1 << 7    //Vblank NMI enable
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

    Byte mRegisters[8]; //registers available for the CPU

    Byte mPShiftReg1;   //for storing 1st bitplane of a tile (LSB)
    Byte mPShiftReg2;   //for storing 2nd bitplane of a tile (MSB)

    Byte mDataBuffer;   //for storing data fetched using PPUADDR register
    Word mVRamAddr;     //for storing PPUADDR address - internal v register
    Byte mWLatch;       //for guarding PPUADDR register functionality - internal w register

    short mRow;         //for keeping track of currently drawn row
    short mColumn;      //for keeping track of currently drawn column
};

#endif // !PPU_H
