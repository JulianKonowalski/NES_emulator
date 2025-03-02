#include "NES/Buses/PPUBus.h"

#include "NES/Cartridge/Cartridge.h"

using Byte = PPUBus::Byte;
using Word = PPUBus::Word;

PPUBus::PPUBus(Cartridge& cartridge) : mCartridge(&cartridge) {}

Byte PPUBus::read(Word address) {
    address &= 0x3FFF; //prevent overflow

    if (address < 0x2000) { return mCartridge->readChrRom(address); }
    if (address < 0x3F00) {

        switch (mCartridge->getMirroringType()) {
        case Mirroring::HORIZONTAL:
            if (address < 2800) { return mNameTable[address & 0x3FF]; }
            else { return mNameTable[0x400 + address & 0x3FF]; }
        case Mirroring::VERTICAL:
            if (address < 2400 || (address >= 2800 && address < 0x2C00)) {
                return mNameTable[address & 0x3FF];
            } else { return mNameTable[0x400 + address & 0x3FF]; }
        default: return mNameTable[address & 0x7FF];
        }

    } else { 
        address &= 0x1F;
        if (address == 0x10) { address = 0x00; }
        if (address == 0x14) { address = 0x04; }
        if (address == 0x18) { address = 0x08; }
        if (address == 0x1C) { address = 0x0C; }
        return mPalettes[address]; 
    }
}

void PPUBus::write(const Byte& data, Word address) {
    address &= 0x3FFF; //prevent overflow

    if (address < 0x2000) { return; }
    else if (address < 0x3F00) {

        switch (mCartridge->getMirroringType()) {
        case Mirroring::HORIZONTAL:
            if (address < 0x2800) { mNameTable[address & 0x3FF] = data; }
            else { mNameTable[0x400 + address & 0x3FF] = data; }
            return;
        case Mirroring::VERTICAL:
            if (address < 2400 || (address >= 2800 && address < 0x2C00)) {
                mNameTable[address & 0x3FF] = data;
            } else { mNameTable[0x400 + address & 0x3FF] = data; }
            return;
        default: mNameTable[address & 0x7FF] = data; return;
        }

    } else { 
        address &= 0x1F;
        if (address == 0x10) { address = 0x00; }
        if (address == 0x14) { address = 0x04; }
        if (address == 0x18) { address = 0x08; }
        if (address == 0x1C) { address = 0x0C; }
        mPalettes[address] = data; 
    }
}