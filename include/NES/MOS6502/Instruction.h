#ifndef  INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <cstdint>

#include "NES/MOS6502/Operation.h"
#include "NES/MOS6502/AddressingMode.h"

class MOS6502;

/**
* To avoid confusion:
* Instruction is a class that consists of 
* an operation and an addressing mode.
* Addressing mode is telling the Instruction
* where it'll find the data (if it needs
* any to execute) and the operation will
* tell it what to do, like executing an
* LDA command.
*/

class Instruction {

	friend class OpcodeLUT; //only LUT can create Instructions and bind them to opcodes

public:

	using Byte = uint8_t;
	using Word = uint16_t;

	void execute(MOS6502& cpu);
	static AddressingMode* getAddressingMode(void) { return sAddressingMode; }

private:

	Instruction(void) :
		mLabel(""),
		mOperation(UndefinedOperation::getInstance()),
		mAddressingMode(UndefinedAddressingMode::getInstance()),
		mCycles(0)
	{}

	Instruction(const std::string& label, Operation* operation, AddressingMode* addresingMode, const Byte& cycles) :
		mLabel(label),
		mOperation(operation),
		mAddressingMode(addresingMode),
		mCycles(cycles)
	{}

	void operator=(const Instruction& other);
	void fetchData(MOS6502& cpu, const Word& address);

	std::string mLabel; //these are useful when debugging, otherwise they don't serve any purpose
	Operation* mOperation;
	AddressingMode* mAddressingMode;
	Byte mCycles;

	static AddressingMode* sAddressingMode;
};

#endif // ! INSTRUCTION_H
