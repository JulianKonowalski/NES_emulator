#include "NES/Nes.h"

#include <functional>

NES::NES(Cartridge& cartridge) :
	mClock(0),
	mWindow(Window::getInstance(mJoypad, ScreenOptions{"NES", 256, 240, 4}, AudioOptions{44100, 16, 1})),
	mApu(mWindow, 44100),
	mPpu(std::bind(&MOS6502::nmi, &mCpu)),
	mCpuBus(mCpu, mPpu, mApu, cartridge, mJoypad, mClock),
	mPpuBus(cartridge)
{
	mCpu.boot(mCpuBus); 
	mPpu.boot(mPpuBus, mWindow);
}

NES::~NES(void) {
	mWindow->destroyInstance();
}

void NES::run(void) {
	while (true) {  
		mPpu.clock();
		if (mClock % 3 == 0) { mCpu.clock(); }
		if (mClock % 6 == 0) { mApu.clock(); }
		++mClock;
	}
}