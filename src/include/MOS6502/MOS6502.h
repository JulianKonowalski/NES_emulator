#ifndef MOS6502_H
#define MOS6502_H

#include <MOS6502/OpcodeLUT.h>
#include <Memory/Memory.h>

enum processorFlag {
	FLAG_CARRY = 1,
	FLAG_ZERO = 1 << 1,
	FLAG_INTERRUPT_DISABLE = 1 << 2,
	FLAG_DECIMAL = 1 << 3,
	FLAG_BREAK = 1 << 4,
	FLAG_OVERFLOW = 1 << 6,
	FLAG_NEGATIVE = 1 << 7
};

class MOS6502 {

	friend class Operation;
	friend class AddressingMode;
	friend class Instruction;

public:

	using Byte = unsigned char;
	using Word = unsigned short;

	MOS6502(void);
	void boot(Memory* memory);
	void executeInstruction(void);

	unsigned int getCycles(void) const { return mCycles; }
	Byte getFetched(void) const { return mFetchedData; }
	Byte getX(void) const { return mX; }
	Byte getY(void) const { return mY; }
	Byte getAccumulator(void) const { return mAccumulator; }
	Byte getProcessorStatus(void) const { return mStatusRegister; }
	Word getProgramCounter(void) const { return mProgramCounter; }

private:

	void init(Memory* memory);
	void readResetVector(void);

	Byte fetchByte(void); //fetch from programCounter address
	Byte fetchByte(const Word& address); //fetch from absolute address

	Byte fetchStack(void);
	void pushStack(const Byte& data);

	void writeMemory(const Byte& data, const Word& address);

	void setFetched(const Byte& data) { mFetchedData = data; }
	void setX(const Byte& data) { mX = data; }
	void setY(const Byte& data) { mY = data; }
	void setFlag(const processorFlag& flag, const bool& value);

	void setAccumulator(const Byte& data) { mAccumulator = data; }
	void setCarryFlag(const bool& state);
	void setZeroFlag(const bool& state);
	void setInterruptFlag(const bool& state);
	void setDecimalFlag(const bool& state);
	void setBreakFlag(const bool& state);
	void setOverflowFlag(const bool& state);
	void setNegativeFlag(const bool& state);

	void setPorcessorStatus(const Byte& status) { mStatusRegister = status; }
	void addCycles(const unsigned short& cycles) { mCycles += cycles; }

	Memory* mMemory;

	OpcodeLUT mOpcodes;

	unsigned int mCycles;

	Byte mFetchedData; //temporary register for storing loaded values

	Word mProgramCounter;
	Byte mStackPointer;
	Byte mAccumulator;
	Byte mX;
	Byte mY;
	Byte mStatusRegister;

};

#endif // !MOS6502_H
