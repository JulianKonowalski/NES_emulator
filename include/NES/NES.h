#ifndef NES_H
#define NES_H

#include <iostream>

#include "MOS6502/MOS6502.h"
#include "Memory/Memory.h"

class NES {
public:

	using Byte = unsigned char;
	using Word = unsigned short;

	NES(const std::string& programPath);
	void run(void);

private:
	void loadProgram(const std::string programPath);

	Memory mMemory;
	MOS6502 mCPU;
};

#endif // !NES_H
