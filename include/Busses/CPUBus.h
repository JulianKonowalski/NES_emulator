#ifndef CPUBUS_H
#define CPUBUS_H

#include <cstdint>

#include "Cartridge/Cartridge.h"

class CPUBus {
public:
    using Byte = uint8_t;
    using Word = uint16_t;

    CPUBus(Cartridge& cartridge);

    Byte read(const Word& address);
    void write(const Word& address, const Byte& data);

private:
    Byte mRam[2048];
    Cartridge* mCartridge;
};

#endif // !CPUBUS_H
