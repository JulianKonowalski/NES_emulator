#include "Cartridge/Cartridge.h"

#include <fstream>
#include <stdexcept>

/**
* There's a lot of 'magic' numbers, but
* they're all well documented on nesdev.org
* in the iNES file format section. All of
* used values are constant across all iNES
* files, so I chose not to create any global
* constants.
*/

Cartridge::Cartridge(const std::string& filePath) : mPrgRomSize(0) {
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

    mMapper = (ctrl2 & 0b11110000) | ctrl1 >> 4;

    //load ROM data into memory
    bool hasTrainer = ctrl1 & 1 << 2;
    Byte dataStart = hasTrainer ? 16 : 16 + 512; //skipping the trainer section
    romFile.seekg(dataStart, romFile.beg);
    for (int i = 0; i < mPrgRom.size(); ++i) { mPrgRom[i] = romFile.get(); }
    for (int i = 0; i < mChrRom.size(); ++i) { mChrRom[i] = romFile.get(); }
}
