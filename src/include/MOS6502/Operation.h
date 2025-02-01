#ifndef OPERATION_H
#define OPERATION_H

#include <stdexcept>

#include <MOS6502/AddressingMode.h>

class MOS6502;
enum processorFlag;

class Operation {
public:

	using Byte = unsigned char;
	using Word = unsigned short;

	void operator=(const Operation& other) = delete;


	virtual void execute(MOS6502& cpu) { /*DO_NOTHING*/ }

protected:

	Operation(void) {}

	void writeMemory(MOS6502& cpu, const Byte& data, const Word& address);
	void pushStack(MOS6502& cpu, const Byte& data);
	void addCpuCycles(MOS6502& cpu, const unsigned short& cycles);
	void setCpuX(MOS6502& cpu, Byte value);
	void setCpuY(MOS6502& cpu, Byte value);
	void setCpuAccumulator(MOS6502& cpu, Byte value);
	void setCpuStatus(MOS6502& cpu, const Byte& status);
	void setCpuStackPointer(MOS6502& cpu, const Byte& value);
	void setCpuFlag(MOS6502& cpu, const processorFlag& flag, const bool& state);

	Byte fetchByte(MOS6502& cpu);
	Byte fetchByte(MOS6502& cpu, const Word& address);
	Byte fetchStack(MOS6502& cpu);

};


class UndefinedOperation : public Operation {
public:

	void operator=(const UndefinedOperation& other) = delete;
	static UndefinedOperation* getInstance(void);
	void execute(MOS6502& cpu);

private:
	UndefinedOperation(void) {}
	static UndefinedOperation* sInstance;
};

/* LOAD/STORE OPERATIONS */


class LDA : public Operation {
public:

	void operator=(const LDA& other) = delete;
	static LDA* getInstance(void);
	void execute(MOS6502& cpu);

private:
	LDA(void) {}
	static LDA* sInstance;
};

class LDX : public Operation {
public:

	void operator=(const LDX& other) = delete;
	static LDX* getInstance(void);
	void execute(MOS6502& cpu);

private:
	LDX(void) {}
	static LDX* sInstance;
};

class LDY : public Operation {
public:

	void operator=(const LDY& other) = delete;
	static LDY* getInstance(void);
	void execute(MOS6502& cpu);

private:
	LDY(void) {}
	static LDY* sInstance;
};

class STA : public Operation {
public:

	void operator=(const STA& other) = delete;
	static STA* getInstance(void);
	void execute(MOS6502& cpu);

private:
	STA(void) {}
	static STA* sInstance;
};

class STX : public Operation {
public:

	void operator=(const STX& other) = delete;
	static STX* getInstance(void);
	void execute(MOS6502& cpu);

private:
	STX(void) {}
	static STX* sInstance;
};

class STY : public Operation {
public:

	void operator=(const STY& other) = delete;
	static STY* getInstance(void);
	void execute(MOS6502& cpu); 

private:
	STY(void) {}
	static STY* sInstance;
};


/* REGISTER TRANSFERS*/


class TAX : public Operation {
public:

	void operator=(const TAX& other) = delete;
	static TAX* getInstance(void);
	void execute(MOS6502& cpu);

private:
	TAX(void) {}
	static TAX* sInstance;
};

class TAY : public Operation {
public:

	void operator=(const TAY& other) = delete;
	static TAY* getInstance(void);
	void execute(MOS6502& cpu);

private:
	TAY(void) {}
	static TAY* sInstance;
};

class TXA : public Operation {
public:

	void operator=(const TXA& other) = delete;
	static TXA* getInstance(void);
	void execute(MOS6502& cpu);

private:
	TXA(void) {}
	static TXA* sInstance;
};

class TYA : public Operation {
public:

	void operator=(const TYA& other) = delete;
	static TYA* getInstance(void);
	void execute(MOS6502& cpu);

private:
	TYA(void) {}
	static TYA* sInstance;
};


/* STACK OPERATIONS */


class TSX : public Operation {
public:

	void operator=(const TSX& other) = delete;
	static TSX* getInstance(void);
	void execute(MOS6502& cpu);

private:
	TSX(void) {}
	static TSX* sInstance;
};

class TXS : public Operation {
public:

	void operator=(const TXS& other) = delete;
	static TXS* getInstance(void);
	void execute(MOS6502& cpu);

private:
	TXS(void) {}
	static TXS* sInstance;
};

class PHA : public Operation {
public:

	void operator=(const PHA& other) = delete;
	static PHA* getInstance(void);
	void execute(MOS6502& cpu);

private:
	PHA(void) {}
	static PHA* sInstance;
};

class PHP : public Operation {
public:

	void operator=(const PHP& other) = delete;
	static PHP* getInstance(void);
	void execute(MOS6502& cpu);

private:
	PHP(void) {}
	static PHP* sInstance;
};

class PLA : public Operation {
public:

	void operator=(const PLA& other) = delete;
	static PLA* getInstance(void);
	void execute(MOS6502& cpu);

private:
	PLA(void) {}
	static PLA* sInstance;
};

class PLP : public Operation {
public:

	void operator=(const PLP& other) = delete;
	static PLP* getInstance(void);
	void execute(MOS6502& cpu);

private:
	PLP(void) {}
	static PLP* sInstance;
};


/* LOGICAL */


class AND : public Operation {
public:

	void operator=(const AND& other) = delete;
	static AND* getInstance(void);
	void execute(MOS6502& cpu);

private:
	AND(void) {}
	static AND* sInstance;
};

class EOR : public Operation {
public:

	void operator=(const EOR& other) = delete;
	static EOR* getInstance(void);
	void execute(MOS6502& cpu);

private:
	EOR(void) {}
	static EOR* sInstance;
};

class ORA : public Operation {
public:

	void operator=(const ORA& other) = delete;
	static ORA* getInstance(void);
	void execute(MOS6502& cpu);

private:
	ORA(void) {}
	static ORA* sInstance;
};

class BIT : public Operation {
public:

	void operator=(const BIT& other) = delete;
	static BIT* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BIT(void) {}
	static BIT* sInstance;
};


/* ARITHMETIC */


class ADC : public Operation {
public:

	void operator=(const ADC& other) = delete;
	static ADC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	ADC(void) {}
	static ADC* sInstance;
};

class SBC : public Operation {
public:

	void operator=(const SBC& other) = delete;
	static SBC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	SBC(void) {}
	static SBC* sInstance;
};

class CMP : public Operation {
public:

	void operator=(const CMP& other) = delete;
	static CMP* getInstance(void);
	void execute(MOS6502& cpu);

private:
	CMP(void) {}
	static CMP* sInstance;
};

class CPX : public Operation {
public:

	void operator=(const BIT& other) = delete;
	static CPX* getInstance(void);
	void execute(MOS6502& cpu);

private:
	CPX(void) {}
	static CPX* sInstance;
};

class CPY : public Operation {
public:

	void operator=(const CPY& other) = delete;
	static CPY* getInstance(void);
	void execute(MOS6502& cpu);

private:
	CPY(void) {}
	static CPY* sInstance;
};


/* INCREMENTS & DECREMENTS */


class INC : public Operation {
public:

void operator=(const INC& other) = delete;
	static INC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	INC(void) {}
	static INC* sInstance;
};

class INX : public Operation {
public:

	void operator=(const INX& other) = delete;
	static INX* getInstance(void);
	void execute(MOS6502& cpu);

private:
	INX(void) {}
	static INX* sInstance;
};

class INY : public Operation {
public:

	void operator=(const INY& other) = delete;
	static INY* getInstance(void);
	void execute(MOS6502& cpu);

private:
	INY(void) {}
	static INY* sInstance;
};

class DEC : public Operation {
public:

	void operator=(const DEC& other) = delete;
	static DEC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	DEC(void) {}
	static DEC* sInstance;
};

class DEX : public Operation {
public:

	void operator=(const DEX& other) = delete;
	static DEX* getInstance(void);
	void execute(MOS6502& cpu);

private:
	DEX(void) {}
	static DEX* sInstance;
};

class DEY : public Operation {
public:

	void operator=(const DEY& other) = delete;
	static DEY* getInstance(void);
	void execute(MOS6502& cpu);

private:
	DEY(void) {}
	static DEY* sInstance;
};


/* SHIFTS */


class ASL : public Operation {
public:

	void operator=(const ASL& other) = delete;
	static ASL* getInstance(void);
	void execute(MOS6502& cpu);

private:
	ASL(void) {}
	static ASL* sInstance;
};

class LSR : public Operation {
public:

	void operator=(const LSR& other) = delete;
	static LSR* getInstance(void);
	void execute(MOS6502& cpu);

private:
	LSR(void) {}
	static LSR* sInstance;
};

class ROL : public Operation {
public:

	void operator=(const ROL& other) = delete;
	static ROL* getInstance(void);
	void execute(MOS6502& cpu);

private:
	ROL(void) {}
	static ROL* sInstance;
};

class ROR : public Operation {
public:

	void operator=(const ROR& other) = delete;
	static ROR* getInstance(void);
	void execute(MOS6502& cpu);

private:
	ROR(void) {}
	static ROR* sInstance;
};


/* JUMPS & CALLS */


class JMP : public Operation {
public:

	void operator=(const JMP& other) = delete;
	static JMP* getInstance(void);
	void execute(MOS6502& cpu);

private:
	JMP(void) {}
	static JMP* sInstance;
};

class JSR : public Operation {
public:

	void operator=(const JSR& other) = delete;
	static JSR* getInstance(void);
	void execute(MOS6502& cpu);

private:
	JSR(void) {}
	static JSR* sInstance;
};

class RTS : public Operation {
public:

	void operator=(const RTS& other) = delete;
	static RTS* getInstance(void);
	void execute(MOS6502& cpu);

private:
	RTS(void) {}
	static RTS* sInstance;
};


/* BRANCHES */


class BCC : public Operation {
public:

	void operator=(const BCC& other) = delete;
	static BCC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BCC(void) {}
	static BCC* sInstance;
};

class BCS : public Operation {
public:

	void operator=(const BCS& other) = delete;
	static BCS* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BCS(void) {}
	static BCS* sInstance;
};

class BEQ : public Operation {
public:

	void operator=(const BEQ& other) = delete;
	static BEQ* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BEQ(void) {}
	static BEQ* sInstance;
};

class BMI : public Operation {
public:

	void operator=(const BMI& other) = delete;
	static BMI* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BMI(void) {}
	static BMI* sInstance;
};

class BNE : public Operation {
public:

	void operator=(const BNE& other) = delete;
	static BNE* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BNE(void) {}
	static BNE* sInstance;
};

class BPL : public Operation {
public:

	void operator=(const BPL& other) = delete;
	static BPL* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BPL(void) {}
	static BPL* sInstance;
};

class BVC : public Operation {
public:

	void operator=(const BVC& other) = delete;
	static BVC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BVC(void) {}
	static BVC* sInstance;
};

class BVS : public Operation {
public:

	void operator=(const BVS& other) = delete;
	static BVS* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BVS(void) {}
	static BVS* sInstance;
};


/* STATUS FLAG CHANGES */


class CLC : public Operation {
public:

	void operator=(const CLC& other) = delete;
	static CLC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	CLC(void) {}
	static CLC* sInstance;
};

class CLD : public Operation {
public:

	void operator=(const CLD& other) = delete;
	static CLD* getInstance(void);
	void execute(MOS6502& cpu);

private:
	CLD(void) {}
	static CLD* sInstance;
};

class CLI : public Operation {
public:

	void operator=(const CLI& other) = delete;
	static CLI* getInstance(void);
	void execute(MOS6502& cpu);

private:
	CLI(void) {}
	static CLI* sInstance;
};

class CLV : public Operation {
public:

	void operator=(const CLV& other) = delete;
	static CLV* getInstance(void);
	void execute(MOS6502& cpu);

private:
	CLV(void) {}
	static CLV* sInstance;
};

class SEC : public Operation {
public:

	void operator=(const SEC& other) = delete;
	static SEC* getInstance(void);
	void execute(MOS6502& cpu);

private:
	SEC(void) {}
	static SEC* sInstance;
};

class SED : public Operation {
public:

	void operator=(const SED& other) = delete;
	static SED* getInstance(void);
	void execute(MOS6502& cpu);

private:
	SED(void) {}
	static SED* sInstance;
};

class SEI : public Operation {
public:

	void operator=(const SEI& other) = delete;
	static SEI* getInstance(void);
	void execute(MOS6502& cpu);

private:
	SEI(void) {}
	static SEI* sInstance;
};


/* SYSTEM FUNCTIONS */

class BRK : public Operation {
public:

	void operator=(const BRK& other) = delete;
	static BRK* getInstance(void);
	void execute(MOS6502& cpu);

private:
	BRK(void) {}
	static BRK* sInstance;
};

class NOP : public Operation {
public:

	void operator=(const NOP&other) = delete;
	static NOP* getInstance(void);
	void execute(MOS6502& cpu);

private:
	NOP(void) {}
	static NOP* sInstance;
};

class RTI : public Operation {
public:

	void operator=(const RTI& other) = delete;
	static RTI* getInstance(void);
	void execute(MOS6502& cpu);

private:
	RTI(void) {}
	static RTI* sInstance;
};

#endif // !OPERATION_H
