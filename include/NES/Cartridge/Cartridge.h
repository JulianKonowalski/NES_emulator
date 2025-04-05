#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>
#include <cstdint>

#include "NES/Cartridge/Mapper.h"

/** 
* Mirroring types. A mirroring 
* type defines the layout of the
* data inside the PPU VRAM.
* 
* @see PPU
*/
enum Mirroring {
    HORIZONTAL,
    VERTICAL,
    ALTERNATIVE
};

/**
* Class representing a NES 
* cartridge. It stores the
* data from the iNES file
* and makes it available to
* outside components.
*/
class Cartridge {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    /**
    * Class constructor. Initializes
    * a class instance with given parameters.
    * 
    * @param filePath path to the iNES file
    *   to be loaded
    */
    Cartridge(const std::string& filePath);

    /**
    * Returns the mirroring type
    * of the cartridge.
    * 
    * @return mirroring type of the
    *   cartridge
    * 
    * @see Mirroring
    */ 
    Mirroring getMirroringType(void) { return mMirroring; }

    /**
    * Returns the data read
    * from the given address 
    * of PRG ROM.
    * 
    * @param address address to
    *   read from
    * 
    * @return data read from the
    *   address
    */
    Byte readPrgRom(const Word& address);

    /**
    * Returns the data read
    * from the given address 
    * of CHR ROM.
    * 
    * @param address address to
    *   read from
    * 
    * @return data read from the
    *   address
    */
    Byte readChrRom(const Word& address);

private:
    /** Cartridge's mapper type */
    Mapper* mMapper;

    /** Cartridge's mirroring type */
    Mirroring mMirroring;

    /** PRG ROM data */
    std::vector<Byte> mPrgRom;

    /** CHR ROM data */
    std::vector<Byte> mChrRom;
};

#endif // !CARTRIDGE_H
