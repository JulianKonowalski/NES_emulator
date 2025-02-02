#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include "memInit.h"

//test suites
#include "AddressingMode_tests.cpp"
#include "Operations/LoadStore_tests.cpp"
#include "Operations/RegisterTransfers_tests.cpp"
#include "Operations/Stack_tests.cpp"
#include "Operations/Logical_tests.cpp"

#include "Operations/IncrementsDecrements_tests.cpp"

TEST(MOS6502_tests, init) {
	MOS6502* cpu = new MOS6502();

	ASSERT_EQ(cpu->getCycles(), 0);
	ASSERT_EQ(cpu->getFetched(), 0);
	ASSERT_EQ(cpu->getX(), 0);
	ASSERT_EQ(cpu->getY(), 0);
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), 0);
	ASSERT_EQ(cpu->getProgramCounter(), 0);

	delete cpu;
}

TEST(MOS6502_tests, bootFromEmptyMemory) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	cpu->boot(memory);

	ASSERT_EQ(cpu->getProgramCounter(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), 0b00100000);

	delete cpu;
	delete memory;
}

TEST(MOS6502_tests, bootFromInitialisedMemory) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	ASSERT_EQ(cpu->getProgramCounter(), 0x1200);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(MOS6502_tests, executeInstruction) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1202);
	ASSERT_EQ(cpu->getAccumulator(), 10);

	delete cpu;
	delete memory;
}