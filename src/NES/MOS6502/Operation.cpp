#include "NES/MOS6502/Operation.h"

#include "NES/MOS6502/MOS6502.h"
#include "NES/MOS6502/Instruction.h"

using Byte = Operation::Byte;
using Word = Operation::Word;

void Operation::writeMemory(MOS6502& cpu, const Byte& data, const Word& address) { cpu.writeMemory(data, address); }
void Operation::pushStack(MOS6502& cpu, const Byte& data) { cpu.pushStack(data); }
void Operation::addCycles(MOS6502& cpu, const Byte& cycles) { cpu.addCycles(cycles); }
void Operation::setCpuX(MOS6502& cpu, const Byte& value) { cpu.setX(value); }
void Operation::setCpuY(MOS6502& cpu, const Byte& value) { cpu.setY(value); }
void Operation::setCpuAccumulator(MOS6502& cpu, const Byte& value) { cpu.setAccumulator(value); }
void Operation::setCpuStatus(MOS6502& cpu, const Byte& status) { cpu.setPorcessorStatus(status); }
void Operation::setCpuStackPointer(MOS6502& cpu, const Byte& value) { cpu.setStackPointer(value); }
void Operation::setCpuProgramCounter(MOS6502& cpu, const Word& value) { cpu.setProgramCounter(value); }
void Operation::setCpuFlag(MOS6502& cpu, const ProcessorFlag& flag, const bool& state) { cpu.setFlag(flag, state); }


Byte Operation::fetchByte(MOS6502& cpu) { return cpu.fetchByte(); }
Byte Operation::fetchByte(MOS6502& cpu, const Word& address) { return cpu.fetchByte(address); }
Byte Operation::fetchStack(MOS6502& cpu) { return cpu.fetchStack(); }


/* MISC */

UndefinedOperation* UndefinedOperation::sInstance = nullptr;
UndefinedOperation* UndefinedOperation::getInstance(void) {
	if (!UndefinedOperation::sInstance) { UndefinedOperation::sInstance = new UndefinedOperation(); }
	return UndefinedOperation::sInstance;
}
void UndefinedOperation::execute(MOS6502& cpu) {
	/* DO NOTHING */
}


/* LOAD/STORE OPERATIONS */


LDA* LDA::sInstance = nullptr;
LDA* LDA::getInstance(void) {
	if (!LDA::sInstance) { LDA::sInstance = new LDA(); }
	return LDA::sInstance;
}
void LDA::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, data);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


LDX* LDX::sInstance = nullptr;
LDX* LDX::getInstance(void) {
	if (!LDX::sInstance) { LDX::sInstance = new LDX(); }
	return LDX::sInstance;
}
void LDX::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuX(cpu, data);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


LDY* LDY::sInstance = nullptr;
LDY* LDY::getInstance(void) {
	if (!LDY::sInstance) { LDY::sInstance = new LDY(); }
	return LDY::sInstance;
}
void LDY::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuY(cpu, data);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


STA* STA::sInstance = nullptr;
STA* STA::getInstance(void) {
	if (!STA::sInstance) { STA::sInstance = new STA(); }
	return STA::sInstance;
}
void STA::execute(MOS6502& cpu) {
	this->writeMemory(cpu, cpu.getAccumulator(), cpu.getFetchedAddress());
}


STX* STX::sInstance = nullptr;
STX* STX::getInstance(void) {
	if (!STX::sInstance) { STX::sInstance = new STX(); }
	return STX::sInstance;
}
void STX::execute(MOS6502& cpu) {
	this->writeMemory(cpu, cpu.getX(), cpu.getFetchedAddress());
}


STY* STY::sInstance = nullptr;
STY* STY::getInstance(void) {
	if (!STY::sInstance) { STY::sInstance = new STY(); }
	return STY::sInstance;
}
void STY::execute(MOS6502& cpu) {
	this->writeMemory(cpu, cpu.getY(), cpu.getFetchedAddress());
}


/* REGISTER TRANSFERS */


TAX* TAX::sInstance = nullptr;
TAX* TAX::getInstance(void) {
	if (!TAX::sInstance) { TAX::sInstance = new TAX(); }
	return TAX::sInstance;
}
void TAX::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, accumulator == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, accumulator & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuX(cpu, accumulator);
}


TAY* TAY::sInstance = nullptr;
TAY* TAY::getInstance(void) {
	if (!TAY::sInstance) { TAY::sInstance = new TAY(); }
	return TAY::sInstance;
}
void TAY::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, accumulator == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, accumulator & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuY(cpu, accumulator);
}


TXA* TXA::sInstance = nullptr;
TXA* TXA::getInstance(void) {
	if (!TXA::sInstance) { TXA::sInstance = new TXA(); }
	return TXA::sInstance;
}
void TXA::execute(MOS6502& cpu) {
	Byte X = cpu.getX();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, X == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, X & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, X);
}


TYA* TYA::sInstance = nullptr;
TYA* TYA::getInstance(void) {
	if (!TYA::sInstance) { TYA::sInstance = new TYA(); }
	return TYA::sInstance;
}
void TYA::execute(MOS6502& cpu) {
	Byte Y = cpu.getY();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, Y == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, Y & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, Y);
}


/* STACK OPERATIONS */


TSX* TSX::sInstance = nullptr;
TSX* TSX::getInstance(void) {
	if (!TSX::sInstance) { TSX::sInstance = new TSX(); }
	return TSX::sInstance;
}
void TSX::execute(MOS6502& cpu) {
	Byte data = cpu.getStackPointer();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuX(cpu, data);
}


TXS* TXS::sInstance = nullptr;
TXS* TXS::getInstance(void) {
	if (!TXS::sInstance) { TXS::sInstance = new TXS(); }
	return TXS::sInstance;
}
void TXS::execute(MOS6502& cpu) {
	this->setCpuStackPointer(cpu, cpu.getX());
}


PHA* PHA::sInstance = nullptr;
PHA* PHA::getInstance(void) {
	if (!PHA::sInstance) { PHA::sInstance = new PHA(); }
	return PHA::sInstance;
}
void PHA::execute(MOS6502& cpu) {
	this->pushStack(cpu, cpu.getAccumulator());
}


PHP* PHP::sInstance = nullptr;
PHP* PHP::getInstance(void) {
	if (!PHP::sInstance) { PHP::sInstance = new PHP(); }
	return PHP::sInstance;
}
void PHP::execute(MOS6502& cpu) {
	//break flag is always added to the pushed copy of processor status
	this->pushStack(cpu, cpu.getProcessorStatus() | ProcessorFlag::FLAG_BREAK);
}


PLA* PLA::sInstance = nullptr;
PLA* PLA::getInstance(void) {
	if (!PLA::sInstance) { PLA::sInstance = new PLA(); }
	return PLA::sInstance;
}
void PLA::execute(MOS6502& cpu) {
	Byte data = this->fetchStack(cpu);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, data);
}


PLP* PLP::sInstance = nullptr;
PLP* PLP::getInstance(void) {
	if (!PLP::sInstance) { PLP::sInstance = new PLP(); }
	return PLP::sInstance;
}
void PLP::execute(MOS6502& cpu) {
	this->setCpuStatus(cpu, this->fetchStack(cpu) | ProcessorFlag::FLAG_DEFAULT);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_BREAK, false);
}

/* LOGICAL */


AND* AND::sInstance = nullptr;
AND* AND::getInstance(void) {
	if (!AND::sInstance) { AND::sInstance = new AND(); }
	return AND::sInstance;
}
void AND::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	Byte result = accumulator & cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, result);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


EOR* EOR::sInstance = nullptr;
EOR* EOR::getInstance(void) {
	if (!EOR::sInstance) { EOR::sInstance = new EOR(); }
	return EOR::sInstance;
}
void EOR::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	Byte result = accumulator ^ cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, result);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


ORA* ORA::sInstance = nullptr;
ORA* ORA::getInstance(void) {
	if (!ORA::sInstance) { ORA::sInstance = new ORA(); }
	return ORA::sInstance;
}
void ORA::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	Byte result = accumulator | cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, result);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


BIT* BIT::sInstance = nullptr;
BIT* BIT::getInstance(void) {
	if (!BIT::sInstance) { BIT::sInstance = new BIT(); }
	return BIT::sInstance;
}
void BIT::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	Byte result = cpu.getAccumulator() & data;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_OVERFLOW, data & ProcessorFlag::FLAG_OVERFLOW);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);
}


/* ARITHMETIC */


ADC* ADC::sInstance = nullptr;
ADC* ADC::getInstance(void) {
	if (!ADC::sInstance) { ADC::sInstance = new ADC(); }
	return ADC::sInstance;
}
void ADC::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	Byte data = cpu.getFetched();
	Word result = accumulator + data;
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_CARRY) { ++result; }

	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, result > 0xff);
	result = (Byte)result;

	//set true if the signs of accumulator and data are the same, but the sign of the result is different
	bool overflow = ~(accumulator & ProcessorFlag::FLAG_NEGATIVE ^ data & ProcessorFlag::FLAG_NEGATIVE) & 
					(accumulator & ProcessorFlag::FLAG_NEGATIVE ^ result & ProcessorFlag::FLAG_NEGATIVE);

	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_OVERFLOW, overflow);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, (Byte)result);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


SBC* SBC::sInstance = nullptr;
SBC* SBC::getInstance(void) {
	if (!SBC::sInstance) { SBC::sInstance = new SBC(); }
	return SBC::sInstance;
}
void SBC::execute(MOS6502& cpu) { //same as ADC but with the binary ~ of fetched data
	Byte accumulator = cpu.getAccumulator();
	Byte data = ~cpu.getFetched();
	Word result = accumulator + data;
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_CARRY) { ++result; }

	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, result > 0xff);
	result = (Byte)result;

	//set true if the signs of accumulator and data are the same, but the sign of the result is different
	bool overflow = ~(accumulator & ProcessorFlag::FLAG_NEGATIVE ^ data & ProcessorFlag::FLAG_NEGATIVE) & 
					(accumulator & ProcessorFlag::FLAG_NEGATIVE ^ result & ProcessorFlag::FLAG_NEGATIVE);

	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_OVERFLOW, overflow);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, (Byte)result);
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
} 


CMP* CMP::sInstance = nullptr;
CMP* CMP::getInstance(void) {
	if (!CMP::sInstance) { CMP::sInstance = new CMP(); }
	return CMP::sInstance;
}
void CMP::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	Byte Accumulator = cpu.getAccumulator();
	Byte result = Accumulator - data;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, Accumulator >= data); //comparison of unsigned values
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & (1 << 7));
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 1); }
}


CPX* CPX::sInstance = nullptr;
CPX* CPX::getInstance(void) {
	if (!CPX::sInstance) { CPX::sInstance = new CPX(); }
	return CPX::sInstance;
}
void CPX::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	Byte X = cpu.getX();
	Byte result = X - data;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, X >= data); //comparison of unsigned values
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & (1 << 7));
}


CPY* CPY::sInstance = nullptr;
CPY* CPY::getInstance(void) {
	if (!CPY::sInstance) { CPY::sInstance = new CPY(); }
	return CPY::sInstance;
}
void CPY::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	Byte Y = cpu.getY();
	Byte result = Y - data;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, Y >= data); //comparison of unsigned values
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & (1 << 7));
}


/* INCREMENTS & DECREMENTS */


INC* INC::sInstance = nullptr;
INC* INC::getInstance(void) {
	if (!INC::sInstance) { INC::sInstance = new INC(); }
	return INC::sInstance;
}
void INC::execute(MOS6502& cpu) {
	Byte result = cpu.getFetched() + 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->writeMemory(cpu, result, cpu.getFetchedAddress());
}


INX* INX::sInstance = nullptr;
INX* INX::getInstance(void) {
	if (!INX::sInstance) { INX::sInstance = new INX(); }
	return INX::sInstance;
}
void INX::execute(MOS6502& cpu) {
	Byte result = cpu.getX() + 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuX(cpu, result);
}


INY* INY::sInstance = nullptr;
INY* INY::getInstance(void) {
	if (!INY::sInstance) { INY::sInstance = new INY(); }
	return INY::sInstance;
}
void INY::execute(MOS6502& cpu) {
	Byte result = cpu.getY() + 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuY(cpu, result);
}


DEC* DEC::sInstance = nullptr;
DEC* DEC::getInstance(void) {
	if (!DEC::sInstance) { DEC::sInstance = new DEC(); }
	return DEC::sInstance;
}
void DEC::execute(MOS6502& cpu) {
	Byte result = cpu.getFetched() - 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->writeMemory(cpu, result, cpu.getFetchedAddress());
}


DEX* DEX::sInstance = nullptr;
DEX* DEX::getInstance(void) {
	if (!DEX::sInstance) { DEX::sInstance = new DEX(); }
	return DEX::sInstance;
}
void DEX::execute(MOS6502& cpu) {
	Byte result = cpu.getX() - 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuX(cpu, result);
}


DEY* DEY::sInstance = nullptr;
DEY* DEY::getInstance(void) {
	if (!DEY::sInstance) { DEY::sInstance = new DEY(); }
	return DEY::sInstance;
}
void DEY::execute(MOS6502& cpu) {
	Byte result = cpu.getY() - 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, result & ProcessorFlag::FLAG_NEGATIVE);
	this->setCpuY(cpu, result);
}


/* SHIFTS */


ASL* ASL::sInstance = nullptr;
ASL* ASL::getInstance(void) {
	if (!ASL::sInstance) { ASL::sInstance = new ASL(); }
	return ASL::sInstance;
}
void ASL::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, data & (1 << 7));

	data <<= 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);

	if (cpu.isAccAddressed()) { this->setCpuAccumulator(cpu, data); } 
	else { this->writeMemory(cpu, data, cpu.getFetchedAddress()); }
}


LSR* LSR::sInstance = nullptr;
LSR* LSR::getInstance(void) {
	if (!LSR::sInstance) { LSR::sInstance = new LSR(); }
	return LSR::sInstance;
}
void LSR::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, data & 1);

	data >>= 1;
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, false); //bit 7 is always 0

	if (cpu.isAccAddressed()) { this->setCpuAccumulator(cpu, data); } 
	else { this->writeMemory(cpu, data, cpu.getFetchedAddress()); }
}


ROL* ROL::sInstance = nullptr;
ROL* ROL::getInstance(void) {
	if (!ROL::sInstance) { ROL::sInstance = new ROL(); }
	return ROL::sInstance;
}
void ROL::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	Byte newCarry = data & (1 << 7);

	data <<= 1;
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_CARRY) { ++data; }

	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, newCarry);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);

	if (cpu.isAccAddressed()) { this->setCpuAccumulator(cpu, data); } 
	else { this->writeMemory(cpu, data, cpu.getFetchedAddress()); }
}


ROR* ROR::sInstance = nullptr;
ROR* ROR::getInstance(void) {
	if (!ROR::sInstance) { ROR::sInstance = new ROR(); }
	return ROR::sInstance;
}
void ROR::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	Byte newCarry = data & 1;

	data >>= 1;
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_CARRY) { data |= (1 << 7); }

	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, newCarry);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_NEGATIVE, data & ProcessorFlag::FLAG_NEGATIVE);

	if (cpu.isAccAddressed()) { this->setCpuAccumulator(cpu, data); } 
	else { this->writeMemory(cpu, data, cpu.getFetchedAddress()); }
}


/* JUMPS & CALLS */


JMP* JMP::sInstance = nullptr;
JMP* JMP::getInstance(void) {
	if (!JMP::sInstance) { JMP::sInstance = new JMP(); }
	return JMP::sInstance;
}
void JMP::execute(MOS6502& cpu) {
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


JSR* JSR::sInstance = nullptr;
JSR* JSR::getInstance(void) {
	if (!JSR::sInstance) { JSR::sInstance = new JSR(); }
	return JSR::sInstance;
}
void JSR::execute(MOS6502& cpu) {
	Word returnAddress = cpu.getProgramCounter() - 1; //the address stored should be target address - 1
	this->pushStack(cpu, returnAddress >> 8);
	this->pushStack(cpu, (Byte)returnAddress);
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


RTS* RTS::sInstance = nullptr;
RTS* RTS::getInstance(void) {
	if (!RTS::sInstance) { RTS::sInstance = new RTS(); }
	return RTS::sInstance;
}
void RTS::execute(MOS6502& cpu) {
	Byte lowByte = this->fetchStack(cpu);
	Word address = (this->fetchStack(cpu) << 8) | lowByte;
	this->setCpuProgramCounter(cpu, ++address); //the address stored is target address - 1
}


/* BRANCHES */


BCC* BCC::sInstance = nullptr;
BCC* BCC::getInstance(void) {
	if (!BCC::sInstance) { BCC::sInstance = new BCC(); }
	return BCC::sInstance;
}
void BCC::execute(MOS6502& cpu) {
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_CARRY) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


BCS* BCS::sInstance = nullptr;
BCS* BCS::getInstance(void) {
	if (!BCS::sInstance) { BCS::sInstance = new BCS(); }
	return BCS::sInstance;
}
void BCS::execute(MOS6502& cpu) {
	if ( !(cpu.getProcessorStatus() & ProcessorFlag::FLAG_CARRY) ) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


BEQ* BEQ::sInstance = nullptr;
BEQ* BEQ::getInstance(void) {
	if (!BEQ::sInstance) { BEQ::sInstance = new BEQ(); }
	return BEQ::sInstance;
}
void BEQ::execute(MOS6502& cpu) {
	if ( !(cpu.getProcessorStatus() & ProcessorFlag::FLAG_ZERO) ) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


BMI* BMI::sInstance = nullptr;
BMI* BMI::getInstance(void) {
	if (!BMI::sInstance) { BMI::sInstance = new BMI(); }
	return BMI::sInstance;
}
void BMI::execute(MOS6502& cpu) {
	if ( !(cpu.getProcessorStatus() & ProcessorFlag::FLAG_NEGATIVE) ) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


BNE* BNE::sInstance = nullptr;
BNE* BNE::getInstance(void) {
	if (!BNE::sInstance) { BNE::sInstance = new BNE(); }
	return BNE::sInstance;
}
void BNE::execute(MOS6502& cpu) {
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_ZERO) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


BPL* BPL::sInstance = nullptr;
BPL* BPL::getInstance(void) {
	if (!BPL::sInstance) { BPL::sInstance = new BPL(); }
	return BPL::sInstance;
}
void BPL::execute(MOS6502& cpu) {
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_NEGATIVE) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


BVC* BVC::sInstance = nullptr;
BVC* BVC::getInstance(void) {
	if (!BVC::sInstance) { BVC::sInstance = new BVC(); }
	return BVC::sInstance;
}
void BVC::execute(MOS6502& cpu) {
	if (cpu.getProcessorStatus() & ProcessorFlag::FLAG_OVERFLOW) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


BVS* BVS::sInstance = nullptr;
BVS* BVS::getInstance(void) {
	if (!BVS::sInstance) { BVS::sInstance = new BVS(); }
	return BVS::sInstance;
}
void BVS::execute(MOS6502& cpu) {
	if ( !(cpu.getProcessorStatus() & ProcessorFlag::FLAG_OVERFLOW) ) { return; }
	if (AddressingMode::pageCrossed()) { this->addCycles(cpu, 2); }
	else { this->addCycles(cpu, 1); }
	this->setCpuProgramCounter(cpu, cpu.getFetchedAddress());
}


/* STATUS FLAG CHANGES */


CLC* CLC::sInstance = nullptr;
CLC* CLC::getInstance(void) {
	if (!CLC::sInstance) { CLC::sInstance = new CLC(); }
	return CLC::sInstance;
}
void CLC::execute(MOS6502& cpu) {
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, false);
}


CLD* CLD::sInstance = nullptr;
CLD* CLD::getInstance(void) {
	if (!CLD::sInstance) { CLD::sInstance = new CLD(); }
	return CLD::sInstance;
}
void CLD::execute(MOS6502& cpu) {
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_DECIMAL, false);
}


CLI* CLI::sInstance = nullptr;
CLI* CLI::getInstance(void) {
	if (!CLI::sInstance) { CLI::sInstance = new CLI(); }
	return CLI::sInstance;
}
void CLI::execute(MOS6502& cpu) {
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_INTERRUPT_DISABLE, false);
}


CLV* CLV::sInstance = nullptr;
CLV* CLV::getInstance(void) {
	if (!CLV::sInstance) { CLV::sInstance = new CLV(); }
	return CLV::sInstance;
}
void CLV::execute(MOS6502& cpu) {
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_OVERFLOW, false);
}


SEC* SEC::sInstance = nullptr;
SEC* SEC::getInstance(void) {
	if (!SEC::sInstance) { SEC::sInstance = new SEC(); }
	return SEC::sInstance;
}
void SEC::execute(MOS6502& cpu) {
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_CARRY, true);
}


SED* SED::sInstance = nullptr;
SED* SED::getInstance(void) {
	if (!SED::sInstance) { SED::sInstance = new SED(); }
	return SED::sInstance;
}
void SED::execute(MOS6502& cpu) {
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_DECIMAL, true);
}


SEI* SEI::sInstance = nullptr;
SEI* SEI::getInstance(void) {
	if (!SEI::sInstance) { SEI::sInstance = new SEI(); }
	return SEI::sInstance;
}
void SEI::execute(MOS6502& cpu) {
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_INTERRUPT_DISABLE, true);
}


/* SYSTEM OPERATIONS */


BRK* BRK::sInstance = nullptr;
BRK* BRK::getInstance(void) {
	if (!BRK::sInstance) { BRK::sInstance = new BRK(); }
	return BRK::sInstance;
}
void BRK::execute(MOS6502& cpu) {
	Word programCounter = cpu.getProgramCounter() + 1; //seems to be a "required" bug
	Byte processorStatus = cpu.getProcessorStatus();
	this->pushStack(cpu, programCounter >> 8);
	this->pushStack(cpu, (Byte)programCounter);
	this->pushStack(cpu, processorStatus | ProcessorFlag::FLAG_BREAK);
	this->setCpuFlag(cpu, ProcessorFlag::FLAG_BREAK, true);

	Byte lowByte = this->fetchByte(cpu, 0xFFFE);
	this->setCpuProgramCounter(cpu, (this->fetchByte(cpu, 0xFFFF) << 8) | lowByte);
	this->setCpuStatus(cpu, processorStatus | ProcessorFlag::FLAG_INTERRUPT_DISABLE);
}


NOP* NOP::sInstance = nullptr;
NOP* NOP::getInstance(void) {
	if (!NOP::sInstance) { NOP::sInstance = new NOP(); }
	return NOP::sInstance;
}
void NOP::execute(MOS6502& cpu) {
	/* DO NOTHING */
}


RTI* RTI::sInstance = nullptr;
RTI* RTI::getInstance(void) {
	if (!RTI::sInstance) { RTI::sInstance = new RTI(); }
	return RTI::sInstance;
}
void RTI::execute(MOS6502& cpu) {
	this->setCpuStatus(cpu, this->fetchStack(cpu));
	Byte lowByte = this->fetchStack(cpu);
	this->setCpuProgramCounter(cpu, (this->fetchStack(cpu) << 8) | lowByte);
}
