#include "NES/Buses/CPUBus.h"

#include <cstring>
#include <stdexcept>

#include "NES/MOS6502/MOS6502.h"

using Byte = CPUBus::Byte;
using Word = CPUBus::Word;

CPUBus::CPUBus(MOS6502& cpu, PPU2C02& ppu, APU& apu, Cartridge& cartridge, Joypad* joypads, Word& globalClock) : 
    mCpu(&cpu),
    mPpu(&ppu), 
    mApu(&apu),
    mCartridge(&cartridge), 
    mDmaWait(false),
    mDmaData(0),
    mGlobalClock(&globalClock)
{
    for(int i = 0; i < 2; ++i) {
        mJoypads[i] = &joypads[i];
        if (!mJoypads[i]) { 
          throw std::runtime_error("Not enough joypads supplied to the CPUBus");
        }
    }
    memset(mRam, 0, 2048); 
}

Byte CPUBus::read(const Word& address) {
    if (address < 0x2000) { return mRam[address & 0x7FF]; } 
    if (address < 0x4000) { return mPpu->readRegister(address); }
    if (address < 0x4020) {
        switch (address) {
          case 0x4016: return mJoypads[0]->read();
          case 0x4017: return mJoypads[1]->read();
          default: return 0;
        }
    } else { return mCartridge->readPrgRom(address); }
}

void CPUBus::write(const Byte& data, const Word& address) {
    if (address < 0x2000) { mRam[address & 0x7FF] = data; } 
    else if (address < 0x4000) { mPpu->writeRegister(data, address); } 
    else if (address < 0x4014) { mApu->writeRegister(data, address); }
    else if (address == 0x4014) { //OAM DMA
        mPpu->startDmaTransfer(data);
        mCpu->startDmaTransfer();
        mDmaWait = true;
    }
    else if (address < 0x4018) { //joypads and APU
        switch (address) {
            case 0x4015: mApu->writeRegister(data, address); break;
            case 0x4016: 
              mJoypads[0]->setStrobe(data == 0x1);
              mJoypads[1]->setStrobe(data == 0x1); 
              break;
            case 0x4017: mApu->writeRegister(data, address); break;
            default: break;
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

    if (mPpu->getOamAddr() == 0x00)         //if the address loops back to 0, end the transfer
        mCpu->stopDmaTransfer();
}
