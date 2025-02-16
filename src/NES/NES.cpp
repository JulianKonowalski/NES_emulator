#include "NES/Nes.h"

#include <fstream>

NES::NES(void) { mCpu.boot(&mCpuBus); }

void NES::run(void) {
	while (true) {  mCpu.clock(); }
}