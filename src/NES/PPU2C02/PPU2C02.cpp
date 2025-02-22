#include "NES/PPU2C02/PPU2C02.h"

#include <iostream>

using Byte = PPU2C02::Byte;
using Word = PPU2C02::Word;

PPU2C02::PPU2C02(void) :
    mScreen(nullptr),
    mBus(nullptr),
    mRow(-1),
    mColumn(0),
    mPShiftReg1(0),
    mPShiftReg2(0),
    mDataBuffer(0),
    mVRamAddr(0),
    mWLatch(0)
{
    for (int i = 0; i < 8; ++i) { mRegisters[i] = 0; }
}

void PPU2C02::boot(PPUBus& bus, Screen& screen) {
    mBus = &bus;
    mScreen = &screen;
}

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
    Byte data(0);
    switch (address &0x7) {
    case PPUCTRL:   break; //write only
    case PPUMASK:   break; //write only
    case PPUSTATUS:
        data = mRegisters[PPUSTATUS];
        //mRegisters[PPUSTATUS] &= ~(1 << 7); //clear the Vblank flag
        mRegisters[PPUSTATUS] |= 1 << 7;
        mWLatch = 0;
        return data;
        break;
    case OAMADDR:   break; //write only
    case OAMDATA:
        break;
    case PPUSCROLL: break; //write only
    case PPUADDR:   break; //write only
    case PPUDATA:
        if (mVRamAddr >= 0x3F00) {  //skip the delay for palette reads 
            data = mBus->read(mVRamAddr);
            ++mVRamAddr; 
            return data;
        }
        data = mDataBuffer;
        mDataBuffer = mBus->read(mVRamAddr);
        mVRamAddr += mRegisters[PPUCTRL] & CTRL_REGISTER::VRAMINC ? 32 : 1;
        return data;
        break;
    }
}

void PPU2C02::writeRegister(const Byte& data, const Word& address) {
    switch (address &0x7) {
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
        if (!mWLatch) { //the high byte of the address is written first
            mVRamAddr = data;
            mWLatch = 1;
        } else {
            mVRamAddr = (mVRamAddr << 8) | data;
            mWLatch = 0;
        }
        break;
    case PPUDATA:
        mBus->write(data, mVRamAddr);
        mVRamAddr += mRegisters[PPUCTRL] & CTRL_REGISTER::VRAMINC ? 32 : 1;
        break;
    }
}
