#include <MOS6502/Operation.h>

#include <MOS6502/MOS6502.h>

void Instruction::operator=(const Instruction& other) {
	mLabel = other.mLabel;
	mOperation = other.mOperation;
	mAddressingMode = other.mAddressingMode;
	mCycles = other.mCycles;
}

void Instruction::fetchData(MOS6502& cpu, const Word& address) {
	cpu.setFetchedAddress(address);
	if (mAddressingMode != IMP::getInstance()) { cpu.setFetched(cpu.fetchByte(address)); }
	else { cpu.setFetched(cpu.getAccumulator()); }
}

void Instruction::execute(MOS6502& cpu) {
	this->fetchData(cpu, mAddressingMode->getAddress(cpu));
	mOperation->execute(cpu);
	cpu.addCycles(mCycles);
}