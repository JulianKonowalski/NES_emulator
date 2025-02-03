#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(JumpCall_tests, opJMP) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x4C;
	(*memory)[0x1201] = 0x00;
	(*memory)[0x1202] = 0x13;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1300);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1300] = 0x6C;
	(*memory)[0x1301] = 0xFF;
	(*memory)[0x1302] = 0x14;
	(*memory)[0x14FF] = 0x34;
	(*memory)[0x1400] = 0x12;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1234);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(JumpCall_tests, opJSR) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x20;
	(*memory)[0x1201] = 0x00;
	(*memory)[0x1202] = 0x13;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1300);
	ASSERT_EQ(cpu->getStackPointer(), 0xFF - 2);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1300] = 0x20;
	(*memory)[0x1301] = 0x00;
	(*memory)[0x1302] = 0x14;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1400);
	ASSERT_EQ(cpu->getStackPointer(), 0xFF - 4);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(JumpCall_tests, opRTS) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x20;
	(*memory)[0x1201] = 0x00;
	(*memory)[0x1202] = 0x13;
	(*memory)[0x1300] = 0x60;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1203);
	ASSERT_EQ(cpu->getStackPointer(), 0xFF);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}