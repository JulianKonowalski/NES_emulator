#include "NES/Buses/PPUBus.h"

#include <cstdlib>

#include "NES/Cartridge/Cartridge.h"

using Byte = PPUBus::Byte;
using Word = PPUBus::Word;

PPUBus::PPUBus(Cartridge& cartridge) : 
    mCartridge(&cartridge) 
{
    memset(mNametable[0], 0, 1024);
    memset(mNametable[1], 0, 1024);
    memset(mPalette, 0, 32);
}

Byte PPUBus::read(Word address) {
    address &= 0x3FFF;
    if (address < 0x2000) {
        return mCartridge->readChrRom(address);
    } else if (address < 0x3F00) {
        address &= 0xFFF;
        switch (mCartridge->getMirroringType()) {
            case Mirroring::HORIZONTAL:
                if (address >= 0x000 && address <= 0x7FF)
                    return mNametable[0][address & 0x3FF];
                if (address >= 0x800 && address <= 0xFFF)
                    return mNametable[1][address & 0x3FF];
                break;
            case Mirroring::VERTICAL:
                if (address >= 0x000 && address <= 0x3FF
                    || address >= 0x800 && address <= 0xBFF) {
                    return mNametable[0][address & 0x3FF];
                }
                if (address >= 0x400 && address <= 0x7FF
                    || address >= 0xC00 && address <= 0xFFF) {
                    return mNametable[1][address & 0x3FF];
                }
                break;
        }
    } else {
        address &= 0x1F;
        if (address == 0x10) { address = 0x00; }
        if (address == 0x14) { address = 0x04; }
        if (address == 0x18) { address = 0x08; }
        if (address == 0x1C) { address = 0x0C; }
        return mPalette[address];
    }
    return 0;
}

void PPUBus::write(const Byte& data, Word address) {
    address &= 0x3FFF;
    if (address < 0x2000) {
        /* DO NOTHING */
    } else if (address < 0x3F00) {
        address &= 0xFFF;
        switch (mCartridge->getMirroringType()) {
            case Mirroring::HORIZONTAL:
                if (address >= 0x000 && address <= 0x7FF) {
                    mNametable[0][address & 0x3FF] = data;
                    return;
                }
                if (address >= 0x800 && address <= 0xFFF) {
                    mNametable[1][address & 0x3FF] = data;
                    return;
                }
                break;
            case Mirroring::VERTICAL:
                if (address >= 0x000 && address <= 0x3FF
                    || address >= 0x800 && address <= 0xBFF) {
                    mNametable[0][address & 0x3FF] = data;
                    return;
                }
                if (address >= 0x400 && address <= 0x7FF
                    || address >= 0xC00 && address <= 0xFFF) {
                    mNametable[1][address & 0x3FF] = data;
                    return;
                }
                break;
        }
    } else {
        address &= 0x1F;
        if (address == 0x10) { address = 0x00; }
        if (address == 0x14) { address = 0x04; }
        if (address == 0x18) { address = 0x08; }
        if (address == 0x1C) { address = 0x0C; }
        mPalette[address] = data;
    }   
}