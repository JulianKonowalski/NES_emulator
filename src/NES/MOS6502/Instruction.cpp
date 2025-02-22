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

void Instruction::fetchAddress(MOS6502& cpu) {
	cpu.setFetchedAddress(mAddressingMode->getAddress(cpu));
	auto addressingModeType = typeid(*mAddressingMode).hash_code();

	//a latch to determine if the data should be fetched from the accumulator
	if (addressingModeType == typeid(*(ACC::getInstance())).hash_code()) {
		cpu.setAccAddressing(true);
	} else {
		cpu.setAccAddressing(false);
	}
}

void Instruction::execute(MOS6502& cpu) {
	this->fetchAddress(cpu);
	mOperation->execute(cpu);
	cpu.addCycles(mCycles);
}