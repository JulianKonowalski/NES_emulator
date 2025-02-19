#include "NES/Cartridge/Cartridge.h"

#include <fstream>
#include <stdexcept>

using Byte = Cartridge::Byte;
using Word = Cartridge::Word;

/**
* There's a lot of 'magic' numbers, but
* they're all well documented on nesdev.org
* in the iNES file format section. All of
* used values are constant across all iNES
* files, so I chose not to create any global
* constants.
*/

Cartridge::Cartridge(const std::string& filePath) {
    std::ifstream romFile(filePath, std::ios::binary);

    //check the opening ASCII string
    const char iNesHeaderStart[4] = { 0x4E, 0x45, 0x53, 0x1A };
    for(int i = 0; i < 4; ++i) {
        if (romFile.get() != iNesHeaderStart[i]) {
            throw std::runtime_error("Error: Unknown file format");
        } 
    }

    //get data section size
    mPrgRom.resize(romFile.get() * 16384);  //size is given in 16KB units
    mChrRom.resize(romFile.get() * 8192);   //size is given in 8KB

    //get additional info about the ROM file
    Byte ctrl1 = romFile.get();
    Byte ctrl2 = romFile.get();

    Byte iNesVer= (ctrl2 & 1 << 3) | (ctrl2 & 1 << 2);
    if (iNesVer == 2) { throw std::runtime_error("Error: NES2.0 format is not supported"); }

    if (ctrl1 & 1 << 3) { mMirroring = Mirroring::ALTERNATIVE; }
    else if (ctrl1 & 1) { mMirroring = Mirroring::VERTICAL; }
    else { mMirroring = Mirroring::HORIZONTAL; }

    Byte mapperId = (ctrl2 & 0b11110000) | ctrl1 >> 4;
    switch (mapperId) {
    case 0:
        mMapper = new Mapper0((Word)mPrgRom.size(), (Word)mChrRom.size());
    break;
    default:
        throw std::runtime_error("Error: Unsupported mapper type");
    }

    //load ROM data into memory
    bool hasTrainer = ctrl1 & 1 << 2;
    Byte dataStart = hasTrainer ? 16 : 16 + 512; //skipping the trainer section
    romFile.seekg(dataStart, romFile.beg);
    for (int i = 0; i < mPrgRom.size(); ++i) { mPrgRom[i] = romFile.get(); }
    for (int i = 0; i < mChrRom.size(); ++i) { mChrRom[i] = romFile.get(); }
}

Byte Cartridge::readPrgRom(const Word& address) {
    return mPrgRom[mMapper->mapPrgRomAddr(address)];
}

Byte Cartridge::readChrRom(const Word& address) {
    return mChrRom[mMapper->mapChrRomAddr(address)];
}
