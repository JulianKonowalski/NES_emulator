#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(Branch_tests, opBCC) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x38;
	(*memory)[0x1201] = 0x90;
	(*memory)[0x1202] = 3;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1203);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY);

	(*memory)[0x1203] = 0x18;
	(*memory)[0x1204] = 0x90;
	(*memory)[0x1205] = 5;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120B);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(Branch_tests, opBCS) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xB0;
	(*memory)[0x1201] = 3;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1202);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0x38;
	(*memory)[0x1203] = 0xB0;
	(*memory)[0x1204] = 5;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120A);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY);

	delete cpu;
	delete memory;
}

TEST(Branch_tests, opBNE) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 0;
	(*memory)[0x1202] = 0xD0;
	(*memory)[0x1203] = 3;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1204);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1204] = 0xA9;
	(*memory)[0x1205] = 1;
	(*memory)[0x1206] = 0xD0;
	(*memory)[0x1207] = 5;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120D);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(Branch_tests, opBEQ) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xF0;
	(*memory)[0x1201] = 3;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1202);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA9;
	(*memory)[0x1203] = 0;
	(*memory)[0x1204] = 0xF0;
	(*memory)[0x1205] = 5;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120B);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	delete cpu;
	delete memory;
}

TEST(Branch_tests, opBPL) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = -1;
	(*memory)[0x1202] = 0x10;
	(*memory)[0x1203] = 3;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1204);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	(*memory)[0x1204] = 0xA9;
	(*memory)[0x1205] = 1;
	(*memory)[0x1206] = 0x10;
	(*memory)[0x1207] = 5;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120D);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(Branch_tests, opBMI) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x30;
	(*memory)[0x1201] = 3;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1202);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA9;
	(*memory)[0x1203] = -1;
	(*memory)[0x1204] = 0x30;
	(*memory)[0x1205] = 5;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120B);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Branch_tests, opBVC) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 127;
	(*memory)[0x1202] = 0x69;
	(*memory)[0x1203] = 1;
	(*memory)[0x1204] = 0x50;
	(*memory)[0x1205] = 2;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1206);
	ASSERT_EQ(cpu->getProcessorStatus(), 
		processorFlag::FLAG_DEFAULT | 
		processorFlag::FLAG_NEGATIVE | 
		processorFlag::FLAG_CARRY | 
		processorFlag::FLAG_OVERFLOW
	);

	(*memory)[0x1206] = 0xB8;
	(*memory)[0x1207] = 0x50;
	(*memory)[0x1208] = 2;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120B);
	ASSERT_EQ(cpu->getProcessorStatus(), 
		processorFlag::FLAG_DEFAULT | 
		processorFlag::FLAG_NEGATIVE | 
		processorFlag::FLAG_CARRY
	);

	delete cpu;
	delete memory;
}

TEST(Branch_tests, opBVS) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x70;
	(*memory)[0x1201] = 3;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x1202);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA9;
	(*memory)[0x1203] = 127;
	(*memory)[0x1204] = 0x69;
	(*memory)[0x1205] = 1;
	(*memory)[0x1206] = 0x70;
	(*memory)[0x1207] = 2;
	cpu->executeInstruction();
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProgramCounter(), 0x120A);
	ASSERT_EQ(cpu->getProcessorStatus(), 
		processorFlag::FLAG_DEFAULT | 
		processorFlag::FLAG_NEGATIVE | 
		processorFlag::FLAG_CARRY | 
		processorFlag::FLAG_OVERFLOW
	);

	delete cpu;
	delete memory;
}
