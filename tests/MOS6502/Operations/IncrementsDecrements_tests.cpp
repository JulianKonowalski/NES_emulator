#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(IncrementsDecrements_tests, opINC) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xE6;
	(*memory)[0x1201] = 0;
	(*memory)[0x0000] = 1;
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 2);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xE6;
	(*memory)[0x1203] = 0;
	(*memory)[0x0000] = -1;
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xE6;
	(*memory)[0x1205] = 0;
	(*memory)[0x0000] = -2;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)(*memory)[0x0000], -1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(IncrementsDecrements_tests, opINX) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xE8;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1201] = 0xA2;
	(*memory)[0x1202] = -1;
	(*memory)[0x1203] = 0xE8;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xA2;
	(*memory)[0x1205] = -2;
	(*memory)[0x1206] = 0xE8;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getX(), -1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(IncrementsDecrements_tests, opINY) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xC8;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1201] = 0xA0;
	(*memory)[0x1202] = -1;
	(*memory)[0x1203] = 0xC8;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xA0;
	(*memory)[0x1205] = -2;
	(*memory)[0x1206] = 0xC8;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getY(), -1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(IncrementsDecrements_tests, opDEC) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xC6;
	(*memory)[0x1201] = 0;
	(*memory)[0x0000] = 2;
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xC6;
	(*memory)[0x1203] = 0;
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xC6;
	(*memory)[0x1205] = 0;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)(*memory)[0x0000], -1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(IncrementsDecrements_tests, opDEX) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2;
	(*memory)[0x1201] = 2;
	(*memory)[0x1202] = 0xCA;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xCA;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xCA;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getX(), -1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(IncrementsDecrements_tests, opDEY) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0;
	(*memory)[0x1201] = 2;
	(*memory)[0x1202] = 0x88;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0x88;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0x88;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getY(), -1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}