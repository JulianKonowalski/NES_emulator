#include "NES/PPU2C02/PPU2C02.h"
 
PPU2C02::PPU2C02(void) :
    mBus(nullptr),
    mScreen(nullptr)
{
    for (int i = 0; i < 8; ++i) { mRegisters[i] = 0; }

    //only this register is not equal to 0 after boot
    mRegisters[PPUSTATUS] = 0b10100000;
}

void PPU2C02::boot(PPUBus& bus, Screen& screen) {
    mBus = &bus;
    mScreen = &screen;
}

void PPU2C02::clock(void) {

}
