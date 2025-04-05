#ifndef  PPUBUS_H
#define PPUBUS_H

#include <cstdint>

#include "NES/Cartridge/Cartridge.h"

/**
* A class that emulates
* the behaviour of an
* internal PPU bus. It 
* connects the PPU to the 
* cartridge and its memory 
* banks like VRAM, Nametables 
* and colour palettes.
* 
* @see PPU
* @see Cartridge
*/
class PPUBus {
public:

    using Byte = uint8_t;
    using Word = uint16_t;
 
    /**
    * Class constructor. Initializes
    * a class instance with given 
    * parameters.
    * 
    * @param cartridge cartridge 
    *   component to be connected
    *   to the bus.
    */
    PPUBus(Cartridge& cartridge);

    /**
    * Returns the data read from
    * the given address.
    * 
    * @param address address to
    *   read from
    * 
    * @return data read from
    *   the given address
    */
    Byte read(Word address);

    /**
    * Writes data to the given address.
    * 
    * @param data data to write
    * @param address address to
    *   write to
    */
    void write(const Byte& data, Word address);

private:

    /** Cartridge component */
    Cartridge* mCartridge;

    /** Colour palettes */
    Byte mPalette[32];

    /** Nametable ram */
    Byte mNametable[2][1024];

};

#endif // ! PPUBUS_H
