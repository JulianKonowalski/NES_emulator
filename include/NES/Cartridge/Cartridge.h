#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>
#include <cstdint>

#include "NES/Cartridge/Mapper.h"

enum Mirroring {
    HORIZONTAL,
    VERTICAL,
    ALTERNATIVE
};

class Cartridge {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    Cartridge(const std::string& filePath);

    Mirroring getMirroringType(void) { return mMirroring; }

    Byte readPrgRom(const Word& address);
    Byte readChrRom(const Word& address);

private:
    Mapper* mMapper;
    Mirroring mMirroring;

    std::vector<Byte> mPrgRom;
    std::vector<Byte> mChrRom;
};

#endif // !CARTRIDGE_H
