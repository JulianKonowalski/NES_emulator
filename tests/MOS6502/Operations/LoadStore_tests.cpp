#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include "../memInit.h"

TEST(LoadStore_tests, opLDA) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;

	cpu->boot(memory);
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA9;
	(*memory)[0x1203] = 0;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xA9;
	(*memory)[0x1205] = -5;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getAccumulator(), -5);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(LoadStore_tests, opLDX) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	(*memory)[0x1200] = 0xA2;
	(*memory)[0x1201] = 10;

	cpu->boot(memory);
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA2;
	(*memory)[0x1203] = 0;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xA2;
	(*memory)[0x1205] = -5;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getX(), -5);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(LoadStore_tests, opLDY) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	(*memory)[0x1200] = 0xA0;
	(*memory)[0x1201] = 10;

	cpu->boot(memory);
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA0;
	(*memory)[0x1203] = 0;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xA0;
	(*memory)[0x1205] = -5;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getY(), -5);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(LoadStore_tests, opSTA) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x85;
	(*memory)[0x1203] = 0;

	cpu->boot(memory);
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(LoadStore_tests, opSTX) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	(*memory)[0x1200] = 0xA2;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x86;
	(*memory)[0x1203] = 0;

	cpu->boot(memory);
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(LoadStore_tests, opSTY) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	(*memory)[0x1200] = 0xA0;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x84;
	(*memory)[0x1203] = 0;

	cpu->boot(memory);
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((*memory)[0x0000], 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}
