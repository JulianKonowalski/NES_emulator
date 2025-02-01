#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(Stack_tests, opTSX) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xBA;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 0xFF);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);
	
	(*memory)[0x1201] = 0xA2;
	(*memory)[0x1202] = 10;
	(*memory)[0x1203] = 0x9A;
	(*memory)[0x1204] = 0xA2;
	(*memory)[0x1205] = 0;
	(*memory)[0x1206] = 0xBA;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1207] = 0xA2;
	(*memory)[0x1208] = 0;
	(*memory)[0x1209] = 0x9A;
	(*memory)[0x120A] = 0xA2;
	(*memory)[0x120B] = 10;
	(*memory)[0x120C] = 0xBA;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	delete cpu;
	delete memory;
}

TEST(Stack_tests, opTXS) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x9A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xA2;
	(*memory)[0x1204] = 0;
	(*memory)[0x1205] = 0x9A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xA2;
	(*memory)[0x1207] = -10;
	(*memory)[0x1208] = 0x9A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getStackPointer(), -10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Stack_tests, opPHA) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x48;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 0xFE);
	ASSERT_EQ((*memory)[0x01FF], 10);

	delete cpu;
	delete memory;
}

TEST(Stack_tests, opPHP) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x08;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 0xFE);
	ASSERT_EQ((*memory)[0x01FF], processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(Stack_tests, opPLA) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x48;
	(*memory)[0x1203] = 0xA9;
	(*memory)[0x1204] = 0;
	(*memory)[0x1205] = 0x68;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 0xFF);
	ASSERT_EQ(cpu->getAccumulator(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1206] = 0xA9;
	(*memory)[0x1207] = 0;
	(*memory)[0x1208] = 0x48;
	(*memory)[0x1209] = 0xA9;
	(*memory)[0x120A] = 10;
	(*memory)[0x120B] = 0x68;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 0xFF);
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x120C] = 0xA9;
	(*memory)[0x120D] = -10;
	(*memory)[0x120E] = 0x48;
	(*memory)[0x120F] = 0xA9;
	(*memory)[0x1210] = 0;
	(*memory)[0x1211] = 0x68;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 0xFF);
	ASSERT_EQ((signed char)cpu->getAccumulator(), -10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Stack_tests, opPLP) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = -10;
	(*memory)[0x1202] = 0x08;
	(*memory)[0x1203] = 0xA9;
	(*memory)[0x1204] = 0;
	(*memory)[0x1205] = 0x28;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getStackPointer(), 0xFF);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}
