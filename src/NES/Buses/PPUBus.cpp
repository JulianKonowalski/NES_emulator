#include "NES/Buses/PPUBus.h"

PPUBus::PPUBus(Cartridge& cartridge) :
    mCartridge(&cartridge)
{
    memset(mNameTable, 2048, 0);
    memset(mPalettes, 32, 0);
}