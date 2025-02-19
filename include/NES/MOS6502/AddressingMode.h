#ifndef ADDRESING_MODE_H
#define ADDRESING_MODE_H

#include <cstdint>

class MOS6502;

//base abstract class
class AddressingMode {
public:

	using Byte = uint8_t;
	using Word = uint16_t;

	void addCycles(MOS6502& cpu, const Byte& cycles);
	void setFetched(MOS6502& cpu, const Byte& data);
	Byte fetchByte(MOS6502& cpu);
	Byte fetchByte(MOS6502& cpu, const Word& address);
	Word fetchFromProgramCounter(MOS6502& cpu);

	virtual Word getAddress(MOS6502& cpu) = 0;
	
	static bool pageCrossed(void) { return AddressingMode::mPageCrossed; }

protected:

	static bool mPageCrossed;
};

//undefined
class UndefinedAddressingMode : public AddressingMode {
public:
	static UndefinedAddressingMode* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	UndefinedAddressingMode(void) = default;
	static UndefinedAddressingMode* sInstance;
};

//accumulator
class ACC : public AddressingMode {
public:
	static ACC* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ACC(void) = default;
	static ACC* sInstance;
};

//implied
class IMP : public AddressingMode {
public:
	static IMP* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IMP(void) = default;
	static IMP* sInstance;
};

//immidiate
class IMM : public AddressingMode {
public:
	static IMM* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IMM(void) = default;
	static IMM* sInstance;
};

//zero page
class ZP0 : public AddressingMode {
public:
	static ZP0* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ZP0(void) = default;
	static ZP0* sInstance;
};

//zero page X
class ZPX : public AddressingMode {
public:
	static ZPX* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ZPX(void) = default;
	static ZPX* sInstance;
};

//zero page Y
class ZPY : public AddressingMode {
public:
	static ZPY* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ZPY(void) = default;
	static ZPY* sInstance;
};

//relative
class REL : public AddressingMode {
public:
	static REL* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	REL(void) = default;
	static REL* sInstance;
};

//absolute
class ABS : public AddressingMode {
public:
	static ABS* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ABS(void) = default;
	static ABS* sInstance;
};

//absolute X
class ABX : public AddressingMode {
public:
	static ABX* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ABX(void) = default;
	static ABX* sInstance;
};

//absolute Y
class ABY : public AddressingMode {
public:
	static ABY* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	ABY(void) = default;
	static ABY* sInstance;
};

//indirect
class IND : public AddressingMode {
public:
	static IND* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IND(void) = default;
	static IND* sInstance;
};

//indexed indirect
class IDX : public AddressingMode {
public:
	static IDX* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IDX(void) = default;
	static IDX* sInstance;
};

//indirect indexed
class IDY : public AddressingMode {
public:
	static IDY* getInstance(void);
	Word getAddress(MOS6502& cpu);

private:
	IDY(void) = default;
	static IDY* sInstance;
};

#endif // !ADDRESING_MODE_H
