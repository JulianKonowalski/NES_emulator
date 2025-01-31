#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include "../memInit.h"

TEST(RegisterTransfers_tests, opTAX) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0xAA;

	cpu->boot(memory);
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xA9;
	(*memory)[0x1204] = 0;
	(*memory)[0x1205] = 0xAA;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getX(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xA9;
	(*memory)[0x1207] = -10;
	(*memory)[0x1208] = 0xAA;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getX(), -10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(RegisterTransfers_tests, opTAY) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();
	
	memInit(memory);
	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0xA8;

	cpu->boot(memory);
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xA9;
	(*memory)[0x1204] = 0;
	(*memory)[0x1205] = 0xA8;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getY(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xA9;
	(*memory)[0x1207] = -10;
	(*memory)[0x1208] = 0xA8;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getY(), -10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(RegisterTransfers_tests, opTXA) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();
	
	memInit(memory);
	(*memory)[0x1200] = 0xA2;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x8A;

	cpu->boot(memory);
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xA2;
	(*memory)[0x1204] = 0;
	(*memory)[0x1205] = 0x8A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xA2;
	(*memory)[0x1207] = -10;
	(*memory)[0x1208] = 0x8A;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getAccumulator(), -10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(RegisterTransfers_tests, opTYA) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();
	
	memInit(memory);
	(*memory)[0x1200] = 0xA0;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0x98;

	cpu->boot(memory);
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1203] = 0xA0;
	(*memory)[0x1204] = 0;
	(*memory)[0x1205] = 0x98;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xA0;
	(*memory)[0x1207] = -10;
	(*memory)[0x1208] = 0x98;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getAccumulator(), -10);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}
