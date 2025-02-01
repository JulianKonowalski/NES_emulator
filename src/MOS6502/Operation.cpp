#include <MOS6502/Operation.h>

#include <MOS6502/MOS6502.h>

using Byte = Operation::Byte;
using Word = Operation::Word;

void Operation::writeMemory(MOS6502& cpu, const Byte& data, const Word& address) { cpu.writeMemory(data, address); }
void Operation::pushStack(MOS6502& cpu, const Byte& data) { cpu.pushStack(data); }
void Operation::setCpuX(MOS6502& cpu, Byte value) { cpu.setX(value); }
void Operation::setCpuY(MOS6502& cpu, Byte value) { cpu.setY(value); }
void Operation::setCpuAccumulator(MOS6502& cpu, Byte value) { cpu.setAccumulator(value); }
void Operation::setCpuStatus(MOS6502& cpu, const Byte& status) { cpu.setPorcessorStatus(status); }
void Operation::setCpuStackPointer(MOS6502& cpu, const Byte& value) { cpu.setStackPointer(value); }
void Operation::setCpuFlag(MOS6502& cpu, const processorFlag& flag, const bool& state) { cpu.setFlag(flag, state); }

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
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, data & processorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, data);
}


LDX* LDX::sInstance = nullptr;
LDX* LDX::getInstance(void) {
	if (!LDX::sInstance) { LDX::sInstance = new LDX(); }
	return LDX::sInstance;
}
void LDX::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, data & processorFlag::FLAG_NEGATIVE);
	this->setCpuX(cpu, data);
}


LDY* LDY::sInstance = nullptr;
LDY* LDY::getInstance(void) {
	if (!LDY::sInstance) { LDY::sInstance = new LDY(); }
	return LDY::sInstance;
}
void LDY::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, data & processorFlag::FLAG_NEGATIVE);
	this->setCpuY(cpu, data);
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
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, accumulator == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, accumulator & processorFlag::FLAG_NEGATIVE);
	this->setCpuX(cpu, accumulator);
}


TAY* TAY::sInstance = nullptr;
TAY* TAY::getInstance(void) {
	if (!TAY::sInstance) { TAY::sInstance = new TAY(); }
	return TAY::sInstance;
}
void TAY::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, accumulator == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, accumulator & processorFlag::FLAG_NEGATIVE);
	this->setCpuY(cpu, accumulator);
}


TXA* TXA::sInstance = nullptr;
TXA* TXA::getInstance(void) {
	if (!TXA::sInstance) { TXA::sInstance = new TXA(); }
	return TXA::sInstance;
}
void TXA::execute(MOS6502& cpu) {
	Byte X = cpu.getX();
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, X == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, X & processorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, X);
}


TYA* TYA::sInstance = nullptr;
TYA* TYA::getInstance(void) {
	if (!TYA::sInstance) { TYA::sInstance = new TYA(); }
	return TYA::sInstance;
}
void TYA::execute(MOS6502& cpu) {
	Byte Y = cpu.getY();
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, Y == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, Y & processorFlag::FLAG_NEGATIVE);
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
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, data & processorFlag::FLAG_NEGATIVE);
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
	this->pushStack(cpu, cpu.getProcessorStatus());
}


PLA* PLA::sInstance = nullptr;
PLA* PLA::getInstance(void) {
	if (!PLA::sInstance) { PLA::sInstance = new PLA(); }
	return PLA::sInstance;
}
void PLA::execute(MOS6502& cpu) {
	Byte data = this->fetchStack(cpu);
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, data == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, data & processorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, data);
}


PLP* PLP::sInstance = nullptr;
PLP* PLP::getInstance(void) {
	if (!PLP::sInstance) { PLP::sInstance = new PLP(); }
	return PLP::sInstance;
}
void PLP::execute(MOS6502& cpu) {
	this->setCpuStatus(cpu, this->fetchStack(cpu));
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
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, result & processorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, result);
}


EOR* EOR::sInstance = nullptr;
EOR* EOR::getInstance(void) {
	if (!EOR::sInstance) { EOR::sInstance = new EOR(); }
	return EOR::sInstance;
}
void EOR::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	Byte result = accumulator ^ cpu.getFetched();
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, result & processorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, result);
}


ORA* ORA::sInstance = nullptr;
ORA* ORA::getInstance(void) {
	if (!ORA::sInstance) { ORA::sInstance = new ORA(); }
	return ORA::sInstance;
}
void ORA::execute(MOS6502& cpu) {
	Byte accumulator = cpu.getAccumulator();
	Byte result = accumulator | cpu.getFetched();
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, result & processorFlag::FLAG_NEGATIVE);
	this->setCpuAccumulator(cpu, result);
}


BIT* BIT::sInstance = nullptr;
BIT* BIT::getInstance(void) {
	if (!BIT::sInstance) { BIT::sInstance = new BIT(); }
	return BIT::sInstance;
}
void BIT::execute(MOS6502& cpu) {
	Byte data = cpu.getFetched();
	Byte result = cpu.getAccumulator() & data;
	this->setCpuFlag(cpu, processorFlag::FLAG_ZERO, result == 0);
	this->setCpuFlag(cpu, processorFlag::FLAG_NEGATIVE, data & processorFlag::FLAG_NEGATIVE);
	this->setCpuFlag(cpu, processorFlag::FLAG_OVERFLOW, data & processorFlag::FLAG_OVERFLOW);
}


/* ARITHMETIC */


ADC* ADC::sInstance = nullptr;
ADC* ADC::getInstance(void) {
	if (!ADC::sInstance) { ADC::sInstance = new ADC(); }
	return ADC::sInstance;
}
void ADC::execute(MOS6502& cpu) {

}


SBC* SBC::sInstance = nullptr;
SBC* SBC::getInstance(void) {
	if (!SBC::sInstance) { SBC::sInstance = new SBC(); }
	return SBC::sInstance;
}
void SBC::execute(MOS6502& cpu) {

}


CMP* CMP::sInstance = nullptr;
CMP* CMP::getInstance(void) {
	if (!CMP::sInstance) { CMP::sInstance = new CMP(); }
	return CMP::sInstance;
}
void CMP::execute(MOS6502& cpu) {

}


CPX* CPX::sInstance = nullptr;
CPX* CPX::getInstance(void) {
	if (!CPX::sInstance) { CPX::sInstance = new CPX(); }
	return CPX::sInstance;
}
void CPX::execute(MOS6502& cpu) {

}


CPY* CPY::sInstance = nullptr;
CPY* CPY::getInstance(void) {
	if (!CPY::sInstance) { CPY::sInstance = new CPY(); }
	return CPY::sInstance;
}
void CPY::execute(MOS6502& cpu) {

}


/* INCREMENTS & DECREMENTS */


INC* INC::sInstance = nullptr;
INC* INC::getInstance(void) {
	if (!INC::sInstance) { INC::sInstance = new INC(); }
	return INC::sInstance;
}
void INC::execute(MOS6502& cpu) {

}


INX* INX::sInstance = nullptr;
INX* INX::getInstance(void) {
	if (!INX::sInstance) { INX::sInstance = new INX(); }
	return INX::sInstance;
}
void INX::execute(MOS6502& cpu) {

}


INY* INY::sInstance = nullptr;
INY* INY::getInstance(void) {
	if (!INY::sInstance) { INY::sInstance = new INY(); }
	return INY::sInstance;
}
void INY::execute(MOS6502& cpu) {

}


DEC* DEC::sInstance = nullptr;
DEC* DEC::getInstance(void) {
	if (!DEC::sInstance) { DEC::sInstance = new DEC(); }
	return DEC::sInstance;
}
void DEC::execute(MOS6502& cpu) {

}


DEX* DEX::sInstance = nullptr;
DEX* DEX::getInstance(void) {
	if (!DEX::sInstance) { DEX::sInstance = new DEX(); }
	return DEX::sInstance;
}
void DEX::execute(MOS6502& cpu) {

}


DEY* DEY::sInstance = nullptr;
DEY* DEY::getInstance(void) {
	if (!DEY::sInstance) { DEY::sInstance = new DEY(); }
	return DEY::sInstance;
}
void DEY::execute(MOS6502& cpu) {

}


/* SHIFTS */


ASL* ASL::sInstance = nullptr;
ASL* ASL::getInstance(void) {
	if (!ASL::sInstance) { ASL::sInstance = new ASL(); }
	return ASL::sInstance;
}
void ASL::execute(MOS6502& cpu) {

}


LSR* LSR::sInstance = nullptr;
LSR* LSR::getInstance(void) {
	if (!LSR::sInstance) { LSR::sInstance = new LSR(); }
	return LSR::sInstance;
}
void LSR::execute(MOS6502& cpu) {

}


ROL* ROL::sInstance = nullptr;
ROL* ROL::getInstance(void) {
	if (!ROL::sInstance) { ROL::sInstance = new ROL(); }
	return ROL::sInstance;
}
void ROL::execute(MOS6502& cpu) {

}


ROR* ROR::sInstance = nullptr;
ROR* ROR::getInstance(void) {
	if (!ROR::sInstance) { ROR::sInstance = new ROR(); }
	return ROR::sInstance;
}
void ROR::execute(MOS6502& cpu) {

}


/* JUMPS & CALLS */


JMP* JMP::sInstance = nullptr;
JMP* JMP::getInstance(void) {
	if (!JMP::sInstance) { JMP::sInstance = new JMP(); }
	return JMP::sInstance;
}
void JMP::execute(MOS6502& cpu) {

}


JSR* JSR::sInstance = nullptr;
JSR* JSR::getInstance(void) {
	if (!JSR::sInstance) { JSR::sInstance = new JSR(); }
	return JSR::sInstance;
}
void JSR::execute(MOS6502& cpu) {

}


RTS* RTS::sInstance = nullptr;
RTS* RTS::getInstance(void) {
	if (!RTS::sInstance) { RTS::sInstance = new RTS(); }
	return RTS::sInstance;
}
void RTS::execute(MOS6502& cpu) {

}


/* BRANCHES */


BCC* BCC::sInstance = nullptr;
BCC* BCC::getInstance(void) {
	if (!BCC::sInstance) { BCC::sInstance = new BCC(); }
	return BCC::sInstance;
}
void BCC::execute(MOS6502& cpu) {

}


BCS* BCS::sInstance = nullptr;
BCS* BCS::getInstance(void) {
	if (!BCS::sInstance) { BCS::sInstance = new BCS(); }
	return BCS::sInstance;
}
void BCS::execute(MOS6502& cpu) {

}


BEQ* BEQ::sInstance = nullptr;
BEQ* BEQ::getInstance(void) {
	if (!BEQ::sInstance) { BEQ::sInstance = new BEQ(); }
	return BEQ::sInstance;
}
void BEQ::execute(MOS6502& cpu) {

}


BMI* BMI::sInstance = nullptr;
BMI* BMI::getInstance(void) {
	if (!BMI::sInstance) { BMI::sInstance = new BMI(); }
	return BMI::sInstance;
}
void BMI::execute(MOS6502& cpu) {

}


BNE* BNE::sInstance = nullptr;
BNE* BNE::getInstance(void) {
	if (!BNE::sInstance) { BNE::sInstance = new BNE(); }
	return BNE::sInstance;
}
void BNE::execute(MOS6502& cpu) {

}


BPL* BPL::sInstance = nullptr;
BPL* BPL::getInstance(void) {
	if (!BPL::sInstance) { BPL::sInstance = new BPL(); }
	return BPL::sInstance;
}
void BPL::execute(MOS6502& cpu) {

}


BVC* BVC::sInstance = nullptr;
BVC* BVC::getInstance(void) {
	if (!BVC::sInstance) { BVC::sInstance = new BVC(); }
	return BVC::sInstance;
}
void BVC::execute(MOS6502& cpu) {

}


BVS* BVS::sInstance = nullptr;
BVS* BVS::getInstance(void) {
	if (!BVS::sInstance) { BVS::sInstance = new BVS(); }
	return BVS::sInstance;
}
void BVS::execute(MOS6502& cpu) {

}

/* STATUS FLAG CHANGES */


CLC* CLC::sInstance = nullptr;
CLC* CLC::getInstance(void) {
	if (!CLC::sInstance) { CLC::sInstance = new CLC(); }
	return CLC::sInstance;
}
void CLC::execute(MOS6502& cpu) {

}


CLD* CLD::sInstance = nullptr;
CLD* CLD::getInstance(void) {
	if (!CLD::sInstance) { CLD::sInstance = new CLD(); }
	return CLD::sInstance;
}
void CLD::execute(MOS6502& cpu) {

}


CLI* CLI::sInstance = nullptr;
CLI* CLI::getInstance(void) {
	if (!CLI::sInstance) { CLI::sInstance = new CLI(); }
	return CLI::sInstance;
}
void CLI::execute(MOS6502& cpu) {

}


CLV* CLV::sInstance = nullptr;
CLV* CLV::getInstance(void) {
	if (!CLV::sInstance) { CLV::sInstance = new CLV(); }
	return CLV::sInstance;
}
void CLV::execute(MOS6502& cpu) {

}


SEC* SEC::sInstance = nullptr;
SEC* SEC::getInstance(void) {
	if (!SEC::sInstance) { SEC::sInstance = new SEC(); }
	return SEC::sInstance;
}
void SEC::execute(MOS6502& cpu) {

}


SED* SED::sInstance = nullptr;
SED* SED::getInstance(void) {
	if (!SED::sInstance) { SED::sInstance = new SED(); }
	return SED::sInstance;
}
void SED::execute(MOS6502& cpu) {

}


SEI* SEI::sInstance = nullptr;
SEI* SEI::getInstance(void) {
	if (!SEI::sInstance) { SEI::sInstance = new SEI(); }
	return SEI::sInstance;
}
void SEI::execute(MOS6502& cpu) {

}


/* SYSTEM OPERATIONS */


BRK* BRK::sInstance = nullptr;
BRK* BRK::getInstance(void) {
	if (!BRK::sInstance) { BRK::sInstance = new BRK(); }
	return BRK::sInstance;
}
void BRK::execute(MOS6502& cpu) {

}


NOP* NOP::sInstance = nullptr;
NOP* NOP::getInstance(void) {
	if (!NOP::sInstance) { NOP::sInstance = new NOP(); }
	return NOP::sInstance;
}
void NOP::execute(MOS6502& cpu) {

}


RTI* RTI::sInstance = nullptr;
RTI* RTI::getInstance(void) {
	if (!RTI::sInstance) { RTI::sInstance = new RTI(); }
	return RTI::sInstance;
}
void RTI::execute(MOS6502& cpu) {

}
