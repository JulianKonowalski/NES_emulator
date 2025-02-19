#include "NES/Cartridge/Mapper.h"

using Byte = Mapper::Byte;
using Word = Mapper::Word;

Word Mapper0::mapPrgRomAddr(const Word& address) {
    if (mPrgRomSize > 0x4000) { return address & 0x7FFF; }
    return address & 0x3FFF;
}

Word Mapper0::mapChrRomAddr(const Word& address) {
    return address;
}