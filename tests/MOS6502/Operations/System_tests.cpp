#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(System_tests, opBRK) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x00;
	(*memory)[0xFFFE] = 0x00;
	(*memory)[0xFFFF] = 0x13;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1300);
	ASSERT_EQ(cpu->getStackPointer(), 0xFF - 3);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_BREAK);

	delete cpu;
	delete memory;
}

TEST(System_tests, opNOP) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xEA;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1201);
	ASSERT_EQ(cpu->getStackPointer(), 0xFF);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(System_tests, opRTI) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x00;
	(*memory)[0xFFFE] = 0x00;
	(*memory)[0xFFFF] = 0x13;
	cpu->executeInstruction();

	(*memory)[0x1300] = 0x40;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1201);
	ASSERT_EQ(cpu->getStackPointer(), 0xFF);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}
