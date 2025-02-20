#include "NES/Buses/PPUBus.h"

using Byte = PPUBus::Byte;
using Word = PPUBus::Word;

PPUBus::PPUBus(Cartridge& cartridge) : mCartridge(&cartridge) {}

Byte PPUBus::read(Word& address) {
    address &= 0x3FFF;
    if (address < 0x2000) {
        //chrom
    } else if (address < 0x3F00) {
        //nametables
    } else if (address < 0x4000) {
        //palettes
    }
    return 0;
}

void PPUBus::write(const Byte& data, Word& address) {
    //ignore
}