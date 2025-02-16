#ifndef NES_H
#define NES_H

#include <iostream>

#include "MOS6502/MOS6502.h"
#include "Busses/CPUBus.h"

class NES {
public:

	using Byte = unsigned char;
	using Word = unsigned short;

	NES(void);
	void run(void);

private:

	CPUBus mCpuBus;
	MOS6502 mCpu;
};

#endif // !NES_H
