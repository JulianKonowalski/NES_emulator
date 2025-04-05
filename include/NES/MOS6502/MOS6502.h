#ifndef MOS6502_H
#define MOS6502_H

#include <cstdint>

#include "NES/MOS6502/OpcodeLUT.h"
#include "NES/Buses/CPUBus.h"

/**
* Masks for all processor
* flags supported by the
* MOS6502 CPU. Their values
* reflect their position
* in a one-byte register.
* 
* @see MOS6502
*/
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
* A class that represents the
* NES' CPU based on a MOS6502.
* Similarly to the actual CPU
* this emulation doesn't 
* support decimal mode.
*/
class MOS6502 {

	friend class Operation;
	friend class Instruction;
	friend class AddressingMode;

public:

	using Byte = uint8_t;
	using Word = uint16_t;

	/**
	* Class constructor. Initializes
	* an instance of the class with
	* default values
	*/
	MOS6502(void);

	/**
	* Boots the CPU by connecting
	* it to a given bus, setting
	* the stack pointer and reading
	* a reset vector from the cartridge.
	* 
	* @param bus bus to be connected
	*	to the CPU
	* 
	* @see CPUBus
	* @see Cartridge
	*/
	void boot(CPUBus& bus);

	/**
	* Clocks the CPU by decreasing
	* the clock counter. When the
	* counter reaches 0 the next 
	* instruction is executed.
	* 
	* @see Instruction
	* @see mCycles
	*/
	void clock(void);

	/**
	* Starts a non-maskable
	* interrupt routine. The CPU
	* reads an NMI vector from the
	* cartridge and starts executing 
	* the given batch of instructions.
	* 
	* @see Cartridge
	*/
	void nmi(void);

	/**
	* Sets the DMA transfer flag to true.
	* While the flag is true the execution
	* of any queued instructions is halted.
	* 
	* @see mDmaTransferOn
	*/
	void startDmaTransfer(void) { mDmaTransferOn = true; }

	/**
	* Clears the DMA transfer flag and
	* the CPU starts executing the queued
	* instructions.
	* 
	* @see mDmaTransferOn
	*/
	void stopDmaTransfer(void) { mDmaTransferOn = false; }

	/**
	* Returns the value of the 
	* temporary fetched data address
	* register.
	* 
	* @return the value of the
	*	temporary fetched data address
	*	register
	* 
	* @see mFetchedAddress
	*/
	Word getFetchedAddress(void) const { return mFetchedAddress; }

	/**
	* Returns the data from
	* the address stored in the
	* temporary fetched data address.
	* 
	* @return fetched data
	* 
	* @see mFetchedAddress
	*/
	Byte getFetched(void) const { return mAccAddressing ? mAccumulator : mBus->read(mFetchedAddress); }

	/**
	* Returns the information
	* if the data for the next
	* operation should be fetched
	* from the accumulator.
	* 
	* @return value of the accumulator
	*	addressing flag
	*/
	bool isAccAddressed(void) { return mAccAddressing; }

	/**
	* Returns the value stored
	* in the cycle counter.
	* 
	* @return value of the cycle
	*	counter
	*/
	Byte getCycles(void) const { return mCycles; }

	/**
	* Returns the data stored
	* in the X register.
	* 
	* @return value of the
	*	X register
	*/
	Byte getX(void) const { return mX; }

	/**
	* Returns the data stored
	* in the Y register.
	* 
	* @return value of the Y
	*	register
	*/
	Byte getY(void) const { return mY; }

	/**
	* Returns the data stored
	* in the accumulator.
	* 
	* @return value of the
	*	accumulator
	*/
	Byte getAccumulator(void) const { return mAccumulator; }

	/**
	* Returns the data stored
	* in the status register.
	* 
	* @return processor status
	*/
	Byte getProcessorStatus(void) const { return mStatusRegister; }
	
	/**
	* Returns the data stored
	* in the stack pointer.
	* 
	* @return value of the
	*	stack pointer
	*/
	Byte getStackPointer(void) const { return mStackPointer; }

	/**
	* Returns the data stored
	* in the program counter.
	* 
	* @return value of the
	*	program counter
	*/
	Word getProgramCounter(void) const { return mProgramCounter; }

private:

	/**
	* Reads the reset vector.
	*/
	void readResetVector(void);

	/**
	* Reads the NMI vector.
	*/
	void readNmiVector(void);

	/**
	* Executes the next instruction
	* pointed to by the program counter.
	*/
	void executeInstruction(void);

	/**
	* Fetches the data from
	* the address pointed
	* to by the program counter.
	* 
	* @return fetched data
	*/
	Byte fetchByte(void);

	/**
	* Fetches the data from
	* a given address.
	* 
	* @param address address
	*	to read the data from
	* 
	* @return fetched data
	*/
	Byte fetchByte(const Word& address);

	/**
	* Fetches a value from
	* the top of the stack.
	* 
	* @return data fetched
	*	from the stack
	*/
	Byte fetchStack(void);

	/**
	* Pushes a given value
	* on the stack.
	* 
	* @param data data to be 
	*	pushed on the stack
	*/
	void pushStack(const Byte& data);

	/**
	* Writes a byte of data
	* to a given address.
	* 
	* @param data data to be 
	*	written
	* @param address address to
	*	write the data to
	*/
	void writeMemory(const Byte& data, const Word& address);

	/**
	* Adds cycles to the
	* CPU cycle counter.
	* 
	* @param cycles amount
	*	of cycles to add to
	*	the counter
	* 
	* @see mCycles
	*/
	void addCycles(const Byte& cycles) { mCycles += cycles; }

	/**
	* Sets the accumulator
	* addressing flag to 
	* a given value.
	* 
	* @param state new state
	*	of the accumulator
	*	addressing flag
	* 
	* @see mAccAddressing
	*/
	void setAccAddressing(const bool& state) { mAccAddressing = state; }

	/**
	* Sets the data address
	* register to a given
	* value.
	* 
	* @param address new value
	*	of the data address
	*	register
	* 
	* @see mFetchedaddress
	*/
	void setFetchedAddress(const Word& address) { mFetchedAddress = address; }

	/**
	* Sets the X register
	* to a given value.
	* 
	* @param data new value
	*	of the X register
	* 
	* @see mX
	*/
	void setX(const Byte& data) { mX = data; }

	/**
	* Sets the Y register
	* to a given value.
	* 
	* @param data new value
	*	of the Y register
	* 
	* @see mY
	*/
	void setY(const Byte& data) { mY = data; }

	/**
	* Sets the accumulator
	* to a given value.
	* 
	* @param data new value
	*	of the accumulator
	* 
	* @see mAccumulator
	*/
	void setAccumulator(const Byte& data) { mAccumulator = data; }

	/**
	* Sets the stack pointer
	* to a given value.
	* 
	* @param data new value
	*	of the stack pointer
	* 
	* @see mStackPointer
	*/
	void setStackPointer(const Byte& data) { mStackPointer = data; }

	/**
	* Sets the program counter
	* to a given value.
	* 
	* @param data new value the
	*	the program counter
	* 
	* @see mProgramCounter
	*/
	void setProgramCounter(const Word& data) { mProgramCounter = data; }

	/**
	* Sets or clears a given
	* flag in the status register.
	* 
	* @param flag flag to set or
	*	clear
	* @param value new value of
	*	the flag
	* 
	* @see mStatusRegister
	*/
	void setFlag(const ProcessorFlag& flag, const bool& value);

	/**
	* Sets the processor status
	* register to a given value.
	* 
	* @param status new status
	*	register value
	* 
	* @see mStatusRegister
	*/
	void setPorcessorStatus(const Byte& status) { mStatusRegister = status | ProcessorFlag::FLAG_DEFAULT; }

	/** Main NES bus */
	CPUBus* mBus;

	/** Lookup table for supported opcodes */
	OpcodeLUT mOpcodes;

	/** Internal cycle counter */
	Byte mCycles;

	/**
	* Temporary register for storing
	* the address of loaded data.
	*/
	Word mFetchedAddress;

	/**
	* A flag to determine if the
	* data should be fetched from
	* the accumulator
	*/
	bool mAccAddressing;

	/**
	* A flag to determine if the
	* DMA transfer is currently
	* running.
	*/
	bool mDmaTransferOn;

	/** Program Counter */
	Word mProgramCounter;

	/** Stack Pointer */
	Byte mStackPointer;

	/** Accumulator register */
	Byte mAccumulator;

	/** X register */
	Byte mX;

	/** Y register */
	Byte mY;

	/** Processor status flags */
	Byte mStatusRegister;

};

#endif // !MOS6502_H
