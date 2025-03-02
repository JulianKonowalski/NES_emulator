#include "NES/Nes.h"

#include <functional>

NES::NES(Cartridge& cartridge) :
    mWindow("NES", 256, 240, 4, mJoypad),
	mClock(0),
	mPpu(std::bind(&MOS6502::nmi, &mCpu)),
	mCpuBus(mPpu, cartridge, mJoypad),
	mPpuBus(cartridge)
{
	mCpu.boot(mCpuBus); 
	mPpu.boot(mPpuBus, mWindow);
}

void NES::run(void) {
	while (true) {  
		mPpu.clock();
		if (mClock % 3 == 0) { mCpu.clock(); }
		++mClock;
	}
}