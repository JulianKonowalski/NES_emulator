#include "NES/Buses/CPUBus.h"

#include <iostream> //delete this
#include <cstring>

using Byte = CPUBus::Byte;
using Word = CPUBus::Word;

CPUBus::CPUBus(PPU2C02& ppu, Cartridge& cartridge, Joypad& joypad) : 
    mPpu(&ppu), 
    mCartridge(&cartridge), 
    mJoypad(&joypad)
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
        default:
            //std::cout << "Unsupported IO device read " << std::hex << (int)address << "\n";
            return 0;
        }
    } else { return mCartridge->readPrgRom(address); } //address >= 0x4020
}

void CPUBus::write(const Byte& data, const Word& address) {
    if (address < 0x2000) { mRam[address & 0x7FF] = data; } 
    else if (address < 0x4000) { mPpu->writeRegister(data, address); } 
    else if (address < 0x4020) {
        switch (address) {
        case 0x4016: mJoypad->setStrobe(data == 0x1); return;
        case 0x4017: return; //ignore the 2nd joypad
        default:
            //std::cout << "Unsupported IO device write " << std::hex << (int)address << "\n";
            return;
        }
    } else { throw std::runtime_error("CPU tried to write into ROM memory\n"); } //address >= 0x4020
}