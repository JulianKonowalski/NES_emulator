#include "Busses/CPUBus.h"

using Byte = CPUBus::Byte;
using Word = CPUBus::Word;

Byte CPUBus::read(Word address) {
    if (address < 0x2000) { return mRAM[address & 0x07FF]; } 
    else if (address < 0x4000) {
        return 0; //PPU registers
    } else if (address < 0x4020) {
        return 0; //APU & IO registers
    } else {
        return 0; //cartridge
    }
}

void CPUBus::write(Word address, Byte data) {
    if (address < 0x2000) { mRAM[address & 0x07FF] = data; } 
    else if (address < 0x4000) {
        //PPU registers
    } else if (address < 0x4020) {
        //APU & IO registers
    } else {
        //cartridge
    }   
}