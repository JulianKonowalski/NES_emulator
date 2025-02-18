#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>
#include <cstdint>

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

    //prgRomSize varies and changes how the bus reads data, so it has a dedicated getter
    Byte getPrgRomSize(void) { return mPrgRomSize; }
    Byte getMapperType(void) { return mMapper; }
    Mirroring getMirroringType(void) { return mMirroring; }

    Byte readPrgRom(const Word& address) { return mPrgRom[address]; }
    Byte readChrRom(const Word& address) { return mChrRom[address]; }

private:
    Byte mMapper;
    Mirroring mMirroring;

    Byte mPrgRomSize;
    std::vector<Byte> mPrgRom;
    std::vector<Byte> mChrRom;
};

#endif // !CARTRIDGE_H
