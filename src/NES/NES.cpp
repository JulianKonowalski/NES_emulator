#include "NES/Nes.h"

#include <fstream>

NES::NES(Cartridge& cartridge) : mCpuBus(cartridge) {
	mCpu.boot(mCpuBus); 
}

void NES::run(void) {
	while (true) {  mCpu.clock(); }
}