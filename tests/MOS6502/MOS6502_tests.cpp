#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>

TEST(MOS6502_tests, init) {
	MOS6502* cpu = new MOS6502();

	ASSERT_EQ(cpu->getCycles(), 0);
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

	//initialise memory
	(*memory)[0xFFFD] = 0x12;	//reset vector
	(*memory)[0xFFFC] = 0x34;	//reset vector

	cpu->boot(memory);

	ASSERT_EQ(cpu->getProgramCounter(), 0x1234);
	ASSERT_EQ(cpu->getProcessorStatus(), 0b00100000);

	delete cpu;
	delete memory;
}

TEST(MOS6502_tests, executeInstruction) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	//initialise memory
	(*memory)[0xFFFD] = 0x12;	//reset vector
	(*memory)[0xFFFC] = 0x00;	//reset vector

	(*memory)[0x1200] = 0xA9;	//LDA command
	(*memory)[0x1201] = 10;		//argument for LDA

	cpu->boot(memory);
	cpu->executeInstruction();

	ASSERT_EQ(cpu->getProgramCounter(), 0x1202);
	ASSERT_EQ(cpu->getAccumulator(), 10);

	//flag changes are tested in operation tests

	delete cpu;
	delete memory;
}