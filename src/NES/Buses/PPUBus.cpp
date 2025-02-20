#include "NES/Buses/PPUBus.h"

using Byte = PPUBus::Byte;
using Word = PPUBus::Word;

PPUBus::PPUBus(Cartridge& cartridge) : mCartridge(&cartridge) {}

Byte PPUBus::read(Word address) {
    address &= 0x3FFF;                      //prevent overflow
    if (address < 0x2000) {
        return mCartridge->readChrRom(address);
    } else if (address < 0x3F00) {
        address &= 0x1FFF;
        return mNameTable[address & 0x7FF]; //simulate memory mirroring
    } else {
        address &= 0x1F;
        return mPalettes[address & 0x0F];   //simulate memory mirroring
    }
}

void PPUBus::write(const Byte& data, Word address) {
    address &= 0x3FFF;                      //prevent overflow
    if (address < 0x2000) {
        //ignore
    } else if (address < 0x3F00) {
        address &= 0x1FFF;
        mNameTable[address & 0x7FF] = data; //simulate memory mirroring
    } else {
        address &= 0x1F;
        mPalettes[address & 0x0F] = data;   //simulate memory mirroring
    }
}