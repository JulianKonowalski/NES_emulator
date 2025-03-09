#ifndef MOS6502_H
#define MOS6502_H

#include <cstdint>

#include "NES/MOS6502/OpcodeLUT.h"
#include "NES/Buses/CPUBus.h"

enum ProcessorFlag {
	FLAG_CARRY = 1,
	FLAG_ZERO = 1 << 1,
	FLAG_INTERRUPT_DISABLE = 1 << 2,
	FLAG_DECIMAL = 1 << 3,
	FLAG_BREAK = 1 << 4,
	FLAG_DEFAULT = 1 << 5,
	FLAG_OVERFLOW = 1 << 6,
	FLAG_NEGATIVE = 1 << 7
};

/**
* Similar to the actual NES
* 6502 processor, this emulation
* does NOT support the decimal
* mode. The decimal flag can
* still be set and cleared, but
* it doesn't affect ADC and SBC
* instructions.
*/

class MOS6502 {

	friend class Operation;
	friend class Instruction;
	friend class AddressingMode;

public:

	using Byte = uint8_t;
	using Word = uint16_t;


	MOS6502(void);
	void boot(CPUBus& bus);
	void clock(void);

	void nmi(void);
	void startDmaTransfer(void) { mDmaTransferOn = true; }
	void stopDmaTransfer(void) { mDmaTransferOn = false; }

	Word getFetchedAddress(void) const { return mFetchedAddress; }
	Byte getFetched(void) const { return mAccAddressing ? mAccumulator : mBus->read(mFetchedAddress); }

	bool isAccAddressed(void) { return mAccAddressing; }

	Byte getCycles(void) const { return mCycles; }

	Byte getX(void) const { return mX; }
	Byte getY(void) const { return mY; }
	Byte getAccumulator(void) const { return mAccumulator; }
	Byte getProcessorStatus(void) const { return mStatusRegister; }
	Byte getStackPointer(void) const { return mStackPointer; }
	Word getProgramCounter(void) const { return mProgramCounter; }

private:

	void readResetVector(void);
	void readNmiVector(void);
	void executeInstruction(void);

	Byte fetchByte(void);					//fetch from programCounter address
	Byte fetchByte(const Word& address);	//fetch from absolute address

	Byte fetchStack(void);
	void pushStack(const Byte& data);

	void writeMemory(const Byte& data, const Word& address);

	void addCycles(const Byte& cycles) { mCycles += cycles; }

	void setAccAddressing(const bool& state) { mAccAddressing = state; }
	void setFetchedAddress(const Word& address) { mFetchedAddress = address; }

	void setX(const Byte& data) { mX = data; }
	void setY(const Byte& data) { mY = data; }
	void setAccumulator(const Byte& data) { mAccumulator = data; }
	void setStackPointer(const Byte& data) { mStackPointer = data; }
	void setProgramCounter(const Word& data) { mProgramCounter = data; }

	void setFlag(const ProcessorFlag& flag, const bool& value);
	void setPorcessorStatus(const Byte& status) { mStatusRegister = status | ProcessorFlag::FLAG_DEFAULT; }

	CPUBus* mBus;

	OpcodeLUT mOpcodes;

	Byte mCycles;

	Word mFetchedAddress;	//temporary register for storing the address of loaded data
	bool mAccAddressing;	//to determine if the data should be fetched from the accumulator

	bool mDmaTransferOn;	//to determine if the DMA transfer is currently running

	Word mProgramCounter;
	Byte mStackPointer;
	Byte mAccumulator;
	Byte mX;
	Byte mY;
	Byte mStatusRegister;

};

#endif // !MOS6502_H
