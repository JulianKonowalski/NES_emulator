#ifndef NES_H
#define NES_H

#include <iostream>

#include "MOS6502/MOS6502.h"
#include "Busses/CPUBus.h"
#include "Cartridge/Cartridge.h"

class NES {
public:

	using Byte = unsigned char;
	using Word = unsigned short;

	NES(Cartridge& cartridge);
	void run(void);

private:

	CPUBus mCpuBus;
	MOS6502 mCpu;
};

#endif // !NES_H
