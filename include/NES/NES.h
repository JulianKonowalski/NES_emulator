#ifndef NES_H
#define NES_H

#include "NES/MOS6502/MOS6502.h"
#include "NES/PPU2C02/PPU2C02.h"
#include "NES/Buses/CPUBus.h"
#include "NES/Buses/PPUBus.h"
#include "NES/APU/APU.h"
#include "NES/Cartridge/Cartridge.h"

#include "IO/Window.h"
#include "IO/Joypad.h"

class NES {
public:

	using Byte = unsigned char;
	using Word = unsigned short;

	NES(Cartridge& cartridge);
	~NES(void);
	void run(void);

private:

	Word mClock;

	Window* mWindow;
	APU mApu;
	MOS6502 mCpu;
	PPU2C02 mPpu;
	CPUBus mCpuBus;
	PPUBus mPpuBus;
	Joypad mJoypad;
};

#endif // !NES_H
