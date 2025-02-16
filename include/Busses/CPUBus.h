#ifndef CPUBUS_H
#define CPUBUS_H

#include <cstdint>

class CPUBus {
public:
    using Byte = uint8_t;
    using Word = uint16_t;

    Byte read(Word address);
    void write(Word address, Byte data);

private:
    Byte mRAM[2048];
};

#endif // !CPUBUS_H
