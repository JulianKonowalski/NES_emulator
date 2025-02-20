#include "NES/PPU2C02/PPU2C02.h"
 
using Byte = PPU2C02::Byte;
using Word = PPU2C02::Word;

PPU2C02::PPU2C02(void) :
    mBus(nullptr),
    mScreen(nullptr),
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

void PPU2C02::clock(void) {
    if (++mColumn >= 341) {
        mColumn = 0;
        ++mRow;
    }

    if (mRow >= 261) {
        mRow = -1;
        mScreen->swapBuffers();
    }

    mScreen->drawPixel(mColumn, mRow, mColours[0x00]);
}

Byte PPU2C02::readRegister(const Word& address) {
    Byte status; //to skip initialisation skipped by case label lol
    switch (address & 0x7) {
    case PPUCTRL: break;    //write only
    case PPUMASK: break;    //write only
    case PPUSTATUS: 
        status = mRegisters[PPUSTATUS];
        mRegisters[PPUSTATUS] &= ~(1 << 7); //clear the Vblank flag
        mAddrLatch = false;
        return status;
    case OAMADDR: break;    //write only
    case OAMDATA: return mRegisters[OAMDATA];
    case PPUSCROLL: break;  //write only
    case PPUADDR: break;    //write only
    case PPUDATA: 
        mRegisters[PPUDATA] = mPpuDataBuffer;
        mPpuDataBuffer = mBus->read(mPpuAddr);
        //skip the delay if the address is reffering to the palettes
        if (mPpuAddr > 0x3f00) { mRegisters[PPUDATA] = mPpuDataBuffer; }
        ++mPpuAddr;
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
            mPpuAddr = 0 | data;
            mAddrLatch = !mAddrLatch;
        } else {
            mPpuAddr = (mPpuAddr << 8) | data;
            mAddrLatch = !mAddrLatch;
        }
        break;
    case PPUDATA:
        mBus->write(data, mPpuAddr);
        ++mPpuAddr;
        break;
    }
}
