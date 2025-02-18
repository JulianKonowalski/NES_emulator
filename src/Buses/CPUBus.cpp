#include "Busses/CPUBus.h"

#include <iostream> //delete this
#include <cstring>

using Byte = CPUBus::Byte;
using Word = CPUBus::Word;

CPUBus::CPUBus(Cartridge& cartridge) : mCartridge(&cartridge) { memset(mRam, 0, 2048); }

Byte CPUBus::read(const Word& address) {
    if (address < 0x2000) { return mRam[address & 0x07FF]; } 
    else if (address < 0x4000) {
        std::cout << "PPU read not supported yet\n";
        return 0;
    } else if (address < 0x4020) {
        std::cout << "APU & IO read not supported yet\n";
        return 0;
    } else {
        //check if the ROM is mirrored
        if (mCartridge->getPrgRomSize() > 0x4000) {
            return mCartridge->readPrgRom(address);
        }
        return mCartridge->readPrgRom(address % 0x4000);
    }
}

void CPUBus::write(const Word& address, const Byte& data) {
    if (address < 0x2000) { mRam[address & 0x07FF] = data; } 
    else if (address < 0x4000) {
        std::cout << "PPU write not supported yet\n";
    } else if (address < 0x4020) {
        std::cout << "APU & IO write not supported yet\n";
    } else {
        throw std::runtime_error("CPU tried to write into ROM memory\n");
    }   
}