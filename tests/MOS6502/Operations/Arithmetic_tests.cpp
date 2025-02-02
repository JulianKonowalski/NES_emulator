#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(Arithmetic_tests, opADC) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x69;
	(*memory)[0x1201] = 1;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1202] = 0xA9;
	(*memory)[0x1203] = 254;
	(*memory)[0x1204] = 0x69;
	(*memory)[0x1205] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 255);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	(*memory)[0x1206] = 0x69;
	(*memory)[0x1207] = 1;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1208] = 0xA9;
	(*memory)[0x1209] = 127;
	(*memory)[0x120A] = 0x69;
	(*memory)[0x120B] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 128);
	ASSERT_EQ(cpu->getProcessorStatus(), 
		processorFlag::FLAG_DEFAULT | 
		processorFlag::FLAG_NEGATIVE | 
		processorFlag::FLAG_CARRY | 
		processorFlag::FLAG_OVERFLOW
	);

	(*memory)[0x120C] = 0xA9;
	(*memory)[0x120D] = 0;
	(*memory)[0x120E] = 0x69;
	(*memory)[0x120F] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 2);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(Arithmetic_tests, opSBC) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);


	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 3;
	(*memory)[0x1202] = 0xE9;
	(*memory)[0x1203] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getAccumulator(), 1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	(*memory)[0x1204] = 0xE9;
	(*memory)[0x1205] = 1;
	cpu->executeInstruction();
	ASSERT_EQ((signed char)cpu->getAccumulator(), -1);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	(*memory)[0x1206] = 0xE9;
	(*memory)[0x1207] = -2;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 0);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_ZERO);

	(*memory)[0x1208] = 0xA9;
	(*memory)[0x1209] = -128;
	(*memory)[0x120A] = 0xE9;
	(*memory)[0x120B] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 126);
	ASSERT_EQ(cpu->getProcessorStatus(), 
		processorFlag::FLAG_DEFAULT |
		processorFlag::FLAG_CARRY |
		processorFlag::FLAG_OVERFLOW
	);

	(*memory)[0x120C] = 0xE9;
	(*memory)[0x120D] = 1;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getAccumulator(), 125);
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(Arithmetic_tests, opCMP) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0xC9;
	(*memory)[0x1203] = 9;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY);
	
	(*memory)[0x1204] = 0xC9;
	(*memory)[0x1205] = 10;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xC9;
	(*memory)[0x1207] = 11;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Arithmetic_tests, opCPX) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0xE0;
	(*memory)[0x1203] = 9;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY);
	
	(*memory)[0x1204] = 0xE0;
	(*memory)[0x1205] = 10;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xE0;
	(*memory)[0x1207] = 11;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}

TEST(Arithmetic_tests, opCPY) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0;
	(*memory)[0x1201] = 10;
	(*memory)[0x1202] = 0xC0;
	(*memory)[0x1203] = 9;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY);
	
	(*memory)[0x1204] = 0xC0;
	(*memory)[0x1205] = 10;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY | processorFlag::FLAG_ZERO);

	(*memory)[0x1206] = 0xC0;
	(*memory)[0x1207] = 11;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_NEGATIVE);

	delete cpu;
	delete memory;
}