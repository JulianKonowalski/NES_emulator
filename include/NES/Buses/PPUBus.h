#ifndef  PPUBUS_H
#define PPUBUS_H

#include <cstdint>

#include "NES/Cartridge/Cartridge.h"

class PPUBus {
public:

    using Byte = uint8_t;
    using Word = uint16_t;
 
    PPUBus(Cartridge& cartridge);

    Byte read(Word address);
    void write(const Byte& data, Word address);

private:

    Cartridge* mCartridge;

    Byte mNameTable[2048] = { 0 };
    Byte mPalettes[32] = { 0 };

};

#endif // ! PPUBUS_H
