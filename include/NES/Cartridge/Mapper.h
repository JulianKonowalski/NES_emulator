#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>

class Mapper {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    Mapper(const Word& prgRomSize, const Word& chrRomSize) : 
        mPrgRomSize(prgRomSize), 
        mChrRomSize(chrRomSize) 
    {}

    virtual Word mapPrgRomAddr(const Word& address) = 0;
    virtual Word mapChrRomAddr(const Word& address) = 0;

protected:
    Word mPrgRomSize;
    Word mChrRomSize;
};

class Mapper0 : public Mapper {
public:
    using Mapper::Mapper;

    Word mapPrgRomAddr(const Word& address);
    Word mapChrRomAddr(const Word& address);
};

#endif // !MAPPER_H
