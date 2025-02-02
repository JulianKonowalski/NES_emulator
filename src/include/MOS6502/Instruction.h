#ifndef  INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

#include <MOS6502/Operation.h>
#include <MOS6502/AddressingMode.h>

class MOS6502;

class Instruction {

	friend class OpcodeLUT; //only this class can create an Operation object

	using Byte = unsigned char;
	using Word = unsigned short;

public:
	void execute(MOS6502& cpu);
	static AddressingMode* getAddressingMode(void) { return sAddressingMode; }

private:

	Instruction(void) :
		mLabel(""),
		mOperation(UndefinedOperation::getInstance()),
		mAddressingMode(UndefinedAddressingMode::getInstance()), //XXX stands for undefined
		mCycles(0)
	{}

	Instruction(const std::string& label, Operation* operation, AddressingMode* addresingMode, const unsigned short& cycles) :
		mLabel(label),
		mOperation(operation),
		mAddressingMode(addresingMode),
		mCycles(cycles)
	{}

	void operator=(const Instruction& other);
	void fetchData(MOS6502& cpu, const Word& address);

	std::string mLabel;
	Operation* mOperation;
	AddressingMode* mAddressingMode;
	unsigned short mCycles;

	static AddressingMode* sAddressingMode;
};

#endif // ! INSTRUCTION_H
