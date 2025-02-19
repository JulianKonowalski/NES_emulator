#include "NES/Nes.h"

#include <fstream>

NES::NES(Screen& screen, Cartridge& cartridge) :
	mClock(0),
	mCpuBus(mPpu, cartridge),
	mPpuBus(cartridge)
{
	mCpu.boot(mCpuBus); 
	mPpu.boot(mPpuBus, screen);
}

void NES::run(void) {
	while (true) {  
		if (mClock % 3 == 0) { mCpu.clock(); }
		mPpu.clock();
		++mClock;
	}
}