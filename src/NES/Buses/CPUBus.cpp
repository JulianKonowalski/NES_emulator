#include "NES/Buses/CPUBus.h"

#include <iostream> //delete this
#include <cstring>

using Byte = CPUBus::Byte;
using Word = CPUBus::Word;

CPUBus::CPUBus(PPU2C02& ppu, Cartridge& cartridge) : mPpu(&ppu), mCartridge(&cartridge) { memset(mRam, 0, 2048); }

Byte CPUBus::read(const Word& address) {
    if (address < 0x2000) { return mRam[address & 0x07FF]; } 
    else if (address < 0x4000) { return mPpu->readRegister(address); } 
    else if (address < 0x4020) {
        std::cout << "APU & IO read not supported yet\n";
        return 0;
    } else { return mCartridge->readPrgRom(address); }
}

void CPUBus::write(const Byte& data, const Word& address) {
    if (address < 0x2000) { mRam[address & 0x07FF] = data; } 
    else if (address < 0x4000) { mPpu->writeRegister(data, address); } 
    else if (address < 0x4020) {
        std::cout << "APU & IO write not supported yet\n";
    } else {
        throw std::runtime_error("CPU tried to write into ROM memory\n");
    }   
}