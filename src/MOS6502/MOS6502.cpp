#include "MOS6502/MOS6502.h"

using Byte = MOS6502::Byte;

MOS6502::MOS6502() : 
	mCycles(0),
	mFetchedData(0),
	mFetchedAddress(0),
	mProgramCounter(0),
	mStackPointer(0xFF),
	mAccumulator(0),
	mX(0),
	mY(0),
	mStatusRegister(processorFlag::FLAG_DEFAULT), //this bit is always set to 1
	mMemory(nullptr)
{};

void MOS6502::boot(Memory* memory) {
	mMemory = memory;
	this->readResetVector();
}

void MOS6502::clock(void) { //execute only when cycle count == 0
	if (!mCycles) { this->executeInstruction(); }
	--mCycles;
}

void MOS6502::readResetVector(void) {
	mProgramCounter = 0x400; //for testing only
	//mProgramCounter = ((*mMemory)[0xFFFD] << 8) | (*mMemory)[0xFFFC];
}

void MOS6502::executeInstruction(void) {
	Byte opcode = (*mMemory)[mProgramCounter];
	++mProgramCounter;
	mOpcodes[opcode].execute(*this);
}


/* MEMORY OPERATIONS */


Byte MOS6502::fetchByte(void) { return (*mMemory)[mProgramCounter++]; }
Byte MOS6502::fetchByte(const Word& address) { return (*mMemory)[address]; }

Byte MOS6502::fetchStack(void) {
	Byte data = (*mMemory)[0x0100 + (++mStackPointer)]; //stkptr will be incremented before the read
	(*mMemory)[mStackPointer] = 0;
	return data;
}
void MOS6502::pushStack(const Byte& data) { (*mMemory)[ 0x0100 + mStackPointer--] = data; }

void MOS6502::writeMemory(const Byte& data, const Word& address) { (*mMemory)[address] = data; }


/* FLAG OPERATIONS */


void MOS6502::setFlag(const processorFlag& flag, const bool& state) {
	if (state) { mStatusRegister |= flag; } 
	else { mStatusRegister &= ~flag; }
}
