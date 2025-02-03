#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include <Memory/Memory.h>
#include "../memInit.h"

TEST(StatusFlag_tests, Carry) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x38;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_CARRY);
	(*memory)[0x1201] = 0x18;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(StatusFlag_tests, Decimal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xF8;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_DECIMAL);
	(*memory)[0x1201] = 0xD8;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(StatusFlag_tests, Interrupt) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x78;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT | processorFlag::FLAG_INTERRUPT_DISABLE);

	(*memory)[0x1201] = 0x58;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), processorFlag::FLAG_DEFAULT);

	delete cpu;
	delete memory;
}

TEST(StatusFlag_tests, Overflow) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA9;
	(*memory)[0x1201] = 127;
	(*memory)[0x1202] = 0x69;
	(*memory)[0x1203] = 1;
	cpu->executeInstruction();
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), 
		processorFlag::FLAG_DEFAULT | 
		processorFlag::FLAG_NEGATIVE | 
		processorFlag::FLAG_CARRY | 
		processorFlag::FLAG_OVERFLOW
	);

	(*memory)[0x1204] = 0xB8;
	cpu->executeInstruction();
	ASSERT_EQ(cpu->getProcessorStatus(), 
		processorFlag::FLAG_DEFAULT | 
		processorFlag::FLAG_NEGATIVE | 
		processorFlag::FLAG_CARRY
	);

	delete cpu;
	delete memory;
}
