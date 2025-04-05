#ifndef ADDRESING_MODE_H
#define ADDRESING_MODE_H

#include <cstdint>

class MOS6502;

/**
* Base class for representing
* MOS6502 addressing modes.
* 
* @see MOS6502
*/
class AddressingMode {
public:

	using Byte = uint8_t;
	using Word = uint16_t;

	/** 
	* Adds cycles to the CPU
	* clock.
	* 
	* @param cpu CPU to add
	*	clock cycles to
	* @param cycles number of
	*	cycles to add
	* 
	* @see MOS6502
	*/
	void addCycles(MOS6502& cpu, const Byte& cycles);

	/**
	* Fetches a byte of data
	* from the address pointed
	* to by the CPU's program
	* counter.
	* 
	* @param cpu CPU to fetch
	*	the data from
	* 
	* @return byte of data fetched
	*	from the address pointed to
	*	by the CPU's program counter
	*/
	Byte fetchByte(MOS6502& cpu);

	/**
	* Fetches a byte of data
	* from the given address.
	* 
	* @param cpu CPU to fetch
	*	the data from
	* @param address address
	*	to fetch the data from
	* 
	* @return byte of data read
	*	from the given address
	*/
	Byte fetchByte(MOS6502& cpu, const Word& address);

	/**
	* Returns the address of the
	* CPU's program counter.
	* 
	* @param cpu CPU to fetch
	*	the program counter address
	*	from
	* 
	* @return CPU's program counter 
	*	address
	*/
	Word fetchFromProgramCounter(MOS6502& cpu);

	/**
	* Returns the parsed address 
	* of the argument of the operation.
	* 
	* @param cpu CPU to fetch the operation
	*	argument address from
	* 
	* @return operation argument address
	*/
	virtual Word getAddress(MOS6502& cpu) = 0;
	
	/**
	* Returns the information
	* if the memory page was
	* crossed during fetching
	* of the data.
	* 
	* @return flag indicating
	*	if a memory page was
	*	crossed
	*/
	static bool pageCrossed(void) { return AddressingMode::mPageCrossed; }

protected:

	/**
	* A flag to indicate if a
	* memory page was crossed
	* during the data fetch.
	*/
	static bool mPageCrossed;
};

/**
* Undefined addressing mode
*/
class UndefinedAddressingMode : public AddressingMode {
public:
	static UndefinedAddressingMode* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	UndefinedAddressingMode(void) = default;
	static UndefinedAddressingMode* sInstance;
};

/**
* Accumulator addressing mode
*/
class ACC : public AddressingMode {
public:
	static ACC* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ACC(void) = default;
	static ACC* sInstance;
};

/**
* Implied addressing mode
*/
class IMP : public AddressingMode {
public:
	static IMP* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IMP(void) = default;
	static IMP* sInstance;
};

/**
* Immediate addressing mode
*/
class IMM : public AddressingMode {
public:
	static IMM* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IMM(void) = default;
	static IMM* sInstance;
};

/**
* Zero Page addressing mode
*/
class ZP0 : public AddressingMode {
public:
	static ZP0* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ZP0(void) = default;
	static ZP0* sInstance;
};

/**
* Zero Page X addressing mode
*/
class ZPX : public AddressingMode {
public:
	static ZPX* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ZPX(void) = default;
	static ZPX* sInstance;
};

/**
* Zero Page Y addressing mode
*/
class ZPY : public AddressingMode {
public:
	static ZPY* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ZPY(void) = default;
	static ZPY* sInstance;
};

/**
* Relative addressing mode
*/
class REL : public AddressingMode {
public:
	static REL* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	REL(void) = default;
	static REL* sInstance;
};

/**
* Absolute addressing mode
*/
class ABS : public AddressingMode {
public:
	static ABS* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ABS(void) = default;
	static ABS* sInstance;
};

/**
* Absolute X addressing mode
*/
class ABX : public AddressingMode {
public:
	static ABX* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ABX(void) = default;
	static ABX* sInstance;
};

/**
* Absolute Y addressing mode
*/
class ABY : public AddressingMode {
public:
	static ABY* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ABY(void) = default;
	static ABY* sInstance;
};

/**
* Indirect addressing mode
*/
class IND : public AddressingMode {
public:
	static IND* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IND(void) = default;
	static IND* sInstance;
};

/**
* Indexed Indirect addressing mode
*/
class IDX : public AddressingMode {
public:
	static IDX* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IDX(void) = default;
	static IDX* sInstance;
};

/**
* Indirect Indexed addressing mode
*/
class IDY : public AddressingMode {
public:
	static IDY* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IDY(void) = default;
	static IDY* sInstance;
};

#endif // !ADDRESING_MODE_H
