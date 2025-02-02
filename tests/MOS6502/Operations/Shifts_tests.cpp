#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(Shifts_tests, opASL) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x06;
	(*memory)[0x1201] = 0x00;
	(*memory)[0x0000] = 1;
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 2);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA9;
	(*memory)[0x1203] = 1;
	(*memory)[0x1204] = 0x0A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 2);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1205] = 0xA9;
	(*memory)[0x1206] = 0b10000000;
	(*memory)[0x1207] = 0x0A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY | processorFlag::FLAG_ZERO);

	(*memory)[0x1208] = 0xA9;
	(*memory)[0x1209] = 0b01100000;
	(*memory)[0x120A] = 0x0A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0b11000000);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Shifts_tests, opLSR) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x46;
	(*memory)[0x1201] = 0x00;
	(*memory)[0x0000] = 2;
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA9;
	(*memory)[0x1203] = 2;
	(*memory)[0x1204] = 0x4A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1205] = 0xA9;
	(*memory)[0x1206] = 0b00000001;
	(*memory)[0x1207] = 0x4A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY | processorFlag::FLAG_ZERO);

	delete cpu;
	delete memory;
}

TEST(Shifts_tests, opROL) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 1;
	(*memory)[0x1202] = 0x2A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 2);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xA9;
	(*memory)[0x1204] = 0b10000000;
	(*memory)[0x1205] = 0x2A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO | processorFlag::FLAG_CARRY);

	(*memory)[0x1206] = 0xA9;
	(*memory)[0x1207] = 0b01000000;
	(*memory)[0x1208] = 0x2A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0b10000001);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Shifts_tests, opROR) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 4;
	(*memory)[0x1202] = 0x6A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 2);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xA9;
	(*memory)[0x1204] = 0b00000001;
	(*memory)[0x1205] = 0x6A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO | processorFlag::FLAG_CARRY);

	(*memory)[0x1206] = 0xA9;
	(*memory)[0x1207] = 0b01000000;
	(*memory)[0x1208] = 0x6A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0b10100000);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}