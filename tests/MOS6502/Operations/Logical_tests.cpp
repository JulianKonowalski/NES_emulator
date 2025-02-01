#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(Logical_tests, opAND) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 1;
	(*memory)[0x1202] = 0x29;
	(*memory)[0x1203] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1204] = 0xA9;
	(*memory)[0x1205] = 1;
	(*memory)[0x1206] = 0x29;
	(*memory)[0x1207] = 2;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1208] = 0xA9;
	(*memory)[0x1209] = 255;
	(*memory)[0x120A] = 0x29;
	(*memory)[0x120B] = 0b10000000;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0b10000000);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Logical_tests, opEOR) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 1;
	(*memory)[0x1202] = 0x49;
	(*memory)[0x1203] = 2;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 3);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1204] = 0xA9;
	(*memory)[0x1205] = 1;
	(*memory)[0x1206] = 0x49;
	(*memory)[0x1207] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1208] = 0xA9;
	(*memory)[0x1209] = 0b10000000;
	(*memory)[0x120A] = 0x49;
	(*memory)[0x120B] = 0b00000001;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0b10000001);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Logical_tests, opORA) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 1;
	(*memory)[0x1202] = 0x09;
	(*memory)[0x1203] = 2;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 3);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1204] = 0xA9;
	(*memory)[0x1205] = 1;
	(*memory)[0x1206] = 0x09;
	(*memory)[0x1207] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);


	(*memory)[0x1208] = 0xA9;
	(*memory)[0x1209] = 0;
	(*memory)[0x120A] = 0x09;
	(*memory)[0x120B] = 0;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x120C] = 0xA9;
	(*memory)[0x120D] = 0b10000000;
	(*memory)[0x120E] = 0x09;
	(*memory)[0x120F] = 0b00000001;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0b10000001);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Logical_tests, opBIT) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 255;
	(*memory)[0x1202] = 0x24;
	(*memory)[0x1203] = 0;
	(*memory)[0x0000] = 0;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0x24;
	(*memory)[0x1205] = 0;
	(*memory)[0x0000] = 0b10000000;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	(*memory)[0x1206] = 0x24;
	(*memory)[0x1207] = 0;
	(*memory)[0x0000] = 0b01000000;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_OVERFLOW);

	delete cpu;
	delete memory;
}