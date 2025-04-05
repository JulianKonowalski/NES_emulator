#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>

/**
* Base class representing
* the NES Cartridge's mapper.
* 
* @see Cartridge
*/
class Mapper {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    /**
    * Class constructor. Initializes
    * a class instance with given parameters.
    * 
    * @param prgRomSize size of PRG ROM
    * @param chrRomSize size of CHR ROM
    */
    Mapper(const Word& prgRomSize, const Word& chrRomSize) : 
        mPrgRomSize(prgRomSize), 
        mChrRomSize(chrRomSize) 
    {}

    /**
    * Maps the given PRG ROM
    * address to a real address
    * on the cartridge.
    * 
    * @param address address to
    *   map
    * 
    * @return mapped address
    */
    virtual Word mapPrgRomAddr(const Word& address) = 0;

    /**
    * Maps the given CHR ROM
    * address to a real address
    * on the cartridge.
    * 
    * @param address address to
    *   map
    * 
    * @return mapped address
    */
    virtual Word mapChrRomAddr(const Word& address) = 0;

protected:

    /** PRG ROM size */
    Word mPrgRomSize;

    /** CHR ROM size */
    Word mChrRomSize;
};

/**
* Class emulating the
* mapper 0 of a NES Cartridge.
* Mapper 0 doesn't perform any
* real mapping and returns the
* address requested by the calling
* component.
*/
class Mapper0 : public Mapper {
public:
    using Mapper::Mapper;

    /**
    * Maps the given PRG ROM
    * address to a real address
    * on the cartridge.
    * 
    * @param address address to
    *   map
    * 
    * @return mapped address
    */
    Word mapPrgRomAddr(const Word& address);

    /**
    * Maps the given CHR ROM
    * address to a real address
    * on the cartridge.
    * 
    * @param address address to
    *   map
    * 
    * @return mapped address
    */
    Word mapChrRomAddr(const Word& address);
};

#endif // !MAPPER_H
