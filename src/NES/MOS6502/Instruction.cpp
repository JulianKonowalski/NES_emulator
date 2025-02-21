#include "NES/MOS6502/Instruction.h"

#include <typeinfo>

#include "NES/MOS6502/MOS6502.h"

AddressingMode* Instruction::sAddressingMode = nullptr;

void Instruction::operator=(const Instruction& other) {
	mLabel = other.mLabel;
	mOperation = other.mOperation;
	mAddressingMode = other.mAddressingMode;
	mCycles = other.mCycles;
}

void Instruction::fetchData(MOS6502& cpu, const Word& address) {
	cpu.setFetchedAddress(address);
	sAddressingMode = mAddressingMode;
	auto addressingModeType = typeid(*mAddressingMode).hash_code();
	if ( addressingModeType == typeid( *(ACC::getInstance()) ).hash_code() ) { 
		cpu.setFetched(cpu.getAccumulator()); 
	} else if ( addressingModeType != typeid( *(IMP::getInstance()) ).hash_code() ) { 
		cpu.setFetched(cpu.fetchByte(address)); 
	}
}

#include <iostream>

void Instruction::execute(MOS6502& cpu) {

	std::cout << typeid(*(mOperation)).name() << " 0x" << std::hex << (int)cpu.getFetched() << "\n";

	this->fetchData(cpu, mAddressingMode->getAddress(cpu));
	mOperation->execute(cpu);
	cpu.addCycles(mCycles);
}