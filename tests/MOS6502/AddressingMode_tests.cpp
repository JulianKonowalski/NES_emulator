#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h>
#endif // !GTEST

#include <MOS6502/MOS6502.h>
#include "memInit.h"

TEST(AddressingMode_tests, undefinedAddressing) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x02; //undefined
	AddressingMode* addressingMode = UndefinedAddressingMode::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, accumulator) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	AddressingMode* addressingMode = ACC::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, implied) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	AddressingMode* addressingMode = IMP::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, immediate) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x02;
	AddressingMode* addressingMode = IMM::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1200);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, zeroPage) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory;

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xFF;
	AddressingMode* addressingMode = ZP0::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x00FF);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, zeroPageXNormal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2; //LDX Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0x06; //test memory address
	cpu->executeInstruction(); //execute LDX call

	AddressingMode* addressingMode = ZPX::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x000A);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, zeroPageXWrapped) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2; //LDX Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0xFF; //test memory address
	cpu->executeInstruction(); //execute LDX call
	AddressingMode* addressingMode = ZPX::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x0003);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, zeroPageYNormal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDY Argument
	(*memory)[0x1202] = 0x06; //test memory address
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = ZPY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x000A);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, zeroPageYWrapped) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDY Argument
	(*memory)[0x1202] = 0xFF; //test memory address
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = ZPY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x0003);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, relativePositive) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 4;
	AddressingMode* addressingMode = REL::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1205);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, relativeNegative) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = -2;
	AddressingMode* addressingMode = REL::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1200);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, absolute) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x34; //address' low byte
	(*memory)[0x1201] = 0x12; //address' high byte
	AddressingMode* addressingMode = ABS::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1234);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, absouluteXNormal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2; //LDX Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0x01; //address' low byte
	(*memory)[0x1203] = 0x12; //address' high byte
	cpu->executeInstruction(); //execute LDX call
	AddressingMode* addressingMode = ABX::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1205);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, absouluteXPageCrossed) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2; //LDX Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0xFF; //address' low byte
	(*memory)[0x1203] = 0x12; //address' high byte
	cpu->executeInstruction(); //execute LDX call
	AddressingMode* addressingMode = ABX::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1303);
	ASSERT_TRUE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, absouluteYNormal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0x01; //address' low byte
	(*memory)[0x1203] = 0x12; //address' high byte
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = ABY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1205);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, absouluteYPageCrossed) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0xFF; //address' low byte
	(*memory)[0x1203] = 0x12; //address' high byte
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = ABY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1303);
	ASSERT_TRUE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectNormal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0x34; //indirect low byte
	(*memory)[0x1201] = 0x12; //indirect high byte
	(*memory)[0x1234] = 0xCD; //direct low byte
	(*memory)[0x1235] = 0xAB; //direct high byte
	AddressingMode* addressingMode = IND::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0xABCD);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectWrapped) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xFF; //indirect low byte
	(*memory)[0x1201] = 0x13; //indirect high byte
	(*memory)[0x13FF] = 0xCD; //direct low byte
	(*memory)[0x1300] = 0xAB; //direct high byte
	AddressingMode* addressingMode = IND::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0xABCD);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectXNormal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2; //LDX Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0x01; //zero page address
	(*memory)[0x0005] = 0xCD; //direct low byte
	(*memory)[0x0006] = 0xAB; //direct high byte
	cpu->executeInstruction(); //execute LDX call
	AddressingMode* addressingMode = IDX::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0xABCD);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectXWrapped) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA2; //LDX Immediate
	(*memory)[0x1201] = 4; //LDX Argument
	(*memory)[0x1202] = 0xFF; //zero page address
	(*memory)[0x0003] = 0xCD; //direct low byte
	(*memory)[0x0004] = 0xAB; //direct high byte
	cpu->executeInstruction(); //execute LDX call
	AddressingMode* addressingMode = IDX::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0xABCD);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectYNormal) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDY Argument
	(*memory)[0x1202] = 0x01; //zero page address
	(*memory)[0x0001] = 0x30; //direct low byte
	(*memory)[0x0002] = 0x12; //direct high byte
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = IDY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1234);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectYWrapped) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDY Argument
	(*memory)[0x1202] = 0xFF; //zero page address
	(*memory)[0x00FF] = 0x30; //direct low byte
	(*memory)[0x0000] = 0x12; //direct high byte
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = IDY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1234);
	ASSERT_FALSE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectYNormalPageCrossed) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDY Argument
	(*memory)[0x1202] = 0x01; //zero page address
	(*memory)[0x0001] = 0xFF; //direct low byte
	(*memory)[0x0002] = 0x12; //direct high byte
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = IDY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1303);
	ASSERT_TRUE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}

TEST(AddressingMode_tests, indirectYWrappedPageCrossed) {
	MOS6502* cpu = new MOS6502();
	Memory* memory = new Memory();

	memInit(memory);
	cpu->boot(memory);

	(*memory)[0x1200] = 0xA0; //LDY Immediate
	(*memory)[0x1201] = 4; //LDY Argument
	(*memory)[0x1202] = 0xFF; //zero page address
	(*memory)[0x00FF] = 0xFF; //direct low byte
	(*memory)[0x0000] = 0x12; //direct high byte
	cpu->executeInstruction(); //execute LDY call
	AddressingMode* addressingMode = IDY::getInstance();
	ASSERT_EQ(addressingMode->getAddress(*cpu), 0x1303);
	ASSERT_TRUE(AddressingMode::pageCrossed());

	delete cpu;
	delete memory;
}
