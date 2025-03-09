#include "NES/Buses/CPUBus.h"

#include <cstring>
#include <stdexcept>

#include "NES/MOS6502/MOS6502.h"

using Byte = CPUBus::Byte;
using Word = CPUBus::Word;

CPUBus::CPUBus(MOS6502& cpu, PPU2C02& ppu, Cartridge& cartridge, Joypad& joypad, Word& globalClock) : 
    mCpu(&cpu),
    mPpu(&ppu), 
    mCartridge(&cartridge), 
    mJoypad(&joypad),
    mDmaWait(false),
    mDmaData(0),
    mGlobalClock(&globalClock)
{ 
    memset(mRam, 0, 2048); 
}

Byte CPUBus::read(const Word& address) {
    if (address < 0x2000) { return mRam[address & 0x7FF]; } 
    if (address < 0x4000) { return mPpu->readRegister(address); } 
    if (address < 0x4020) {
        switch (address) {
        case 0x4016: return mJoypad->read();
        case 0x4017: return 0; //ignore the 2nd joypad
        default: return 0;
        }
    } else { return mCartridge->readPrgRom(address); }
}

void CPUBus::write(const Byte& data, const Word& address) {
    if (address < 0x2000) { mRam[address & 0x7FF] = data; } 
    else if (address < 0x4000) { mPpu->writeRegister(data, address); } 
    else if (address < 0x4020) {
        switch (address) {
            case 0x4014:
                mPpu->startDmaTransfer(data);
                mCpu->startDmaTransfer();
                mDmaWait = true;
                return;
            case 0x4016: mJoypad->setStrobe(data == 0x1); return;
            case 0x4017: return; //ignore the 2nd joypad
            default: return;
        }
    } else { throw std::runtime_error("CPU tried to write into ROM memory\n"); }
}

void CPUBus::dmaTransfer(void) {
    if (mDmaWait) { //wait at the start to sync everything
        if (*mGlobalClock % 2)
            mDmaWait = false;
        return;
    }

    if (*mGlobalClock % 2 == 0) {           //fetch data on even cycles
        Word ramAddr = mPpu->getOamDma() << 8 | mPpu->getOamAddr();
        mDmaData = mRam[ramAddr & 0x7FF];
        return;
    } else { mPpu->writeDma(mDmaData); }    //write data on odd cycles

    if (mPpu->getOamAddr() == 0x00) //if the address loops back to 0, end the transfer
        mCpu->stopDmaTransfer();
}