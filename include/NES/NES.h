#ifndef NES_H
#define NES_H

#include <iostream>

#include "NES/MOS6502/MOS6502.h"
#include "NES/PPU2C02/PPU2C02.h"
#include "NES/Buses/CPUBus.h"
#include "NES/Buses/PPUBus.h"
#include "NES/Cartridge/Cartridge.h"
#include "IO/Screen.h"

class NES {
public:

	using Byte = unsigned char;
	using Word = unsigned short;

	NES(Screen& screen, Cartridge& cartridge);
	void run(void);

private:

	Word mClock;

	MOS6502 mCpu;
	PPU2C02 mPpu;
	CPUBus mCpuBus;
	PPUBus mPpuBus;
};

#endif // !NES_H
