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

    Byte mPalette[32];
    Byte mNametable[2][1024];

};

#endif // ! PPUBUS_H
