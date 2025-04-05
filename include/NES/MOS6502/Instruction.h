#ifndef  INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <cstdint>

#include "NES/MOS6502/Operation.h"
#include "NES/MOS6502/AddressingMode.h"

class MOS6502;

/**
* Class that consists of 
* an operation and an 
* addressing mode.
* 
* @see AddressingMode
* @see Operation
*/
class Instruction {

	friend class OpcodeLUT;

public:

	using Byte = uint8_t;
	using Word = uint16_t;

	/**
	* Executes and operation
	* stored in the instruction.
	* 
	* @param cpu CPU that executes
	*	the operation
	* 
	* @see MOS6502
	*/
	void execute(MOS6502& cpu);

	/**
	* Returns the type of
	* addressing mode of
	* the operation.
	* 
	* @return operation's
	*	addressing mode
	*/
	static AddressingMode* getAddressingMode(void) { return sAddressingMode; }

private:

	/**
	* Class constructor. Creates
	* an instance of the class
	* with default parameters.
	*/
	Instruction(void) :
		mLabel(""),
		mOperation(UndefinedOperation::getInstance()),
		mAddressingMode(UndefinedAddressingMode::getInstance()),
		mCycles(0)
	{}

	/**
	* Class constructor. Creates
	* an instance of the class
	* with given parameters.
	* 
	* @param label a label of
	*	the instruction
	* @param operation operation
	*	type of the instruction
	* @param addressingMode addressing
	*	mode type of the instruction
	* @param cycles cost of the instruction
	*	in CPU cycles
	*/
	Instruction(const std::string& label, Operation* operation, AddressingMode* addresingMode, const Byte& cycles) :
		mLabel(label),
		mOperation(operation),
		mAddressingMode(addresingMode),
		mCycles(cycles)
	{}

	void operator=(const Instruction& other);

	/**
	* Fetches the address of the
	* argument of the operation.
	* 
	* @param cpu CPU that executes
	*	the instruction
	*/
	void fetchAddress(MOS6502& cpu);

	/** Instruction label */
	std::string mLabel;

	/** Operation type of the instruction */
	Operation* mOperation;

	/** Addressing mode type of the instruction */
	AddressingMode* mAddressingMode;

	/** Cost of the operation in CPU cycles */
	Byte mCycles;

	/** Pointer to the addressing mode type */
	static AddressingMode* sAddressingMode;
};

#endif // ! INSTRUCTION_H
