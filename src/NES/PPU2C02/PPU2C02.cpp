#include "NES/PPU2C02/PPU2C02.h"
 
#include <iostream> //detete this

using Byte = PPU2C02::Byte;
using Word = PPU2C02::Word;

PPU2C02::PPU2C02(void) :
    mBus(nullptr),
    mScreen(nullptr),
    mPShiftReg1(0),
    mPShiftReg2(0),
    mPpuDataBuffer(0),
    mPpuAddr(0),
    mAddrLatch(false),
    mRow(-1),
    mColumn(0)
{
    for (int i = 0; i < 8; ++i) { mRegisters[i] = 0; }

    //only this register is not equal to 0 after boot
    mRegisters[PPUSTATUS] = 0b10100000;
}

void PPU2C02::boot(PPUBus& bus, Screen& screen) {
    mBus = &bus;
    mScreen = &screen;
}


/**
* This method provides the
* whole drawing logic. It's
* a bit long, but I wanted
* to keep all of it in one
* place, as it can be quite
* confusing.
*/

void PPU2C02::clock(void) {

    if (mColumn % 8 == 0) { //fetch new tile data only when crossing the tile border
        Word spriteOffset = mRegisters[PPUCTRL] & CTRL_REGISTER::SPRADDR ? 0x1000 : 0;

        Word tileIndexAddress = 0x2000 + (mColumn / 8) + ( (mRow / 8) * 32 );   //base addres + tile's column index + (tile's row index * number of columns)
        Byte tileIndex = mBus->read(tileIndexAddress);

        //NES fetches 16 bytes of tile data, I'm only fetching 2 needed for the current line
        Word patternDataAddress = spriteOffset + (tileIndex * 16) + ( (mRow % 8) ); //offset + (tile * tileSize) + row index within the tile

        mPShiftReg1 = mBus->read(patternDataAddress);
        mPShiftReg2 = mBus->read(patternDataAddress + 8);
    }

    Byte pixelCode = (mPShiftReg1 >> 7) + (mPShiftReg2 >> 7);
    mPShiftReg1 <<= 1;
    mPShiftReg2 <<= 1;

    Byte paletteCode = 0;
    Byte colourCode = mBus->read(0x3F00 + (paletteCode << 2) + pixelCode);

    mScreen->drawPixel(mColumn, mRow, mColours[colourCode]);

    if (++mColumn >= 341) {   //NES draws scanlines longer than 256 pixels
        mColumn = 0;
        if (++mRow >= 261) {  //NES also draws more than 240 scanlines
            mRow = -1;
            mScreen->swapBuffers();
        }
    }

}

Byte PPU2C02::readRegister(const Word& address) {
    Byte status; //to avoid "initialisation skipped by case label" error lol
    switch (address & 0x7) {
    case PPUCTRL: break;    //write only
    case PPUMASK: break;    //write only
    case PPUSTATUS:
        status = mRegisters[PPUSTATUS];
        //mRegisters[PPUSTATUS] &= ~(1 << 7); //clear the Vblank flag
        mRegisters[PPUSTATUS] |= 1 << 7;
        mAddrLatch = false;
        return status;
    case OAMADDR: break;    //write only
    case OAMDATA: return mRegisters[OAMDATA];
    case PPUSCROLL: break;  //write only
    case PPUADDR: break;    //write only
    case PPUDATA:
        //skip the delay if the address is reffering to the palettes
        if (mPpuAddr >= 0x3F00) { ++mPpuAddr; return mBus->read(mPpuAddr); }
        mRegisters[PPUDATA] = mPpuDataBuffer;
        mPpuDataBuffer = mBus->read(mPpuAddr);
        mPpuAddr += mRegisters[PPUCTRL] & CTRL_REGISTER::VRAMINC ? 32 : 1;

        std::cout << "PPUDATA read increment. Returned " << (int)mRegisters[PPUDATA] << "\n";

        return mRegisters[PPUDATA];
    }
}

void PPU2C02::writeRegister(const Byte& data, const Word& address) {
    switch (address & 0x7) {
    case PPUCTRL:
        mRegisters[PPUCTRL] = data;
        break;
    case PPUMASK:
        mRegisters[PPUMASK] = data;
        break;
    case PPUSTATUS: break; //read only
    case OAMADDR:
        break;
    case OAMDATA:
        break;
    case PPUSCROLL:
        break;
    case PPUADDR:
        if (!mAddrLatch) { //here the byte storage is NOT little endian
            mPpuAddr = data;
            mAddrLatch = !mAddrLatch;
        } else {
            mPpuAddr = (mPpuAddr << 8) | data;
            mAddrLatch = !mAddrLatch;
            std::cout << "Set PPUADDR to " << (int)mPpuAddr << "\n";
        }
        break;
    case PPUDATA:
        mBus->write(data, mPpuAddr);

        std::cout << "PPUDATA write increment. Wrote " << (int)data << "\n";

        //mPpuAddr += mRegisters[PPUCTRL] & CTRL_REGISTER::VRAMINC ? 32 : 1;

        break;
    }
}
