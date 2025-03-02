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

#include <iostream>
#include <ios>
#include <fstream>

std::ofstream logfile;

static long cycles = 0;
bool logging = false;

void Instruction::execute(MOS6502& cpu) {


	auto pc = cpu.getProgramCounter() - 1;

	//if (logging) {
		//std::cout << std::hex << (int)pc << " " << typeid(*(mOperation)).name() << "\n";
	//}

	this->fetchAddress(cpu);
	mOperation->execute(cpu);
	cpu.addCycles(mCycles);

	//if (!logfile.is_open()) { logfile.open("ctrlLog.txt", std::ios::trunc); }
	//if (logfile.is_open()) {
	//	logfile << 
	//		std::hex << (int)pc << " " << 
	//		std::hex << (int)cpu.getAccumulator() << " " << 
	//		std::hex << (int)cpu.getX() << " " << 
	//		std::hex << (int)cpu.getY() << " " << 
	//		std::hex << (int)cpu.getProcessorStatus() << " " << 
	//		std::hex << (int)cpu.getStackPointer() << "\n";
	//}

	//cycles++;
}