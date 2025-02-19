#ifndef  PPUBUS_H
#define PPUBUS_H

#include <cstdint>

#include "NES/Cartridge/Cartridge.h"

class PPUBus {
public:

    using Byte = uint8_t;
    using Word = uint16_t;
 
    PPUBus(Cartridge& cartridge);

    Byte read(const Word& address);
    void write(const Byte& data, const Word& address);

private:

    Cartridge* mCartridge;

    Byte mNameTable[2048];
    Byte mPalettes[32];

};

#endif // ! PPUBUS_H
