#include <MOS6502/MOS6502.h>

using Byte = MOS6502::Byte;

MOS6502::MOS6502() : 
	mCycles(0),
	mFetchedData(0),
	mProgramCounter(0),
	mStackPointer(0),
	mAccumulator(0),
	mX(0),
	mY(0),
	mStatusRegister(0),
	mMemory(nullptr)
{};

void MOS6502::boot(Memory* memory) {
	this->init(memory);
	this->readResetVector();
}

void MOS6502::init(Memory* memory) {
	mCycles = mAccumulator = mX = mY = 0;
	mStackPointer = 0xFF;
	mStatusRegister = processorFlag::FLAG_DEFAULT;
	mMemory = memory;
}

void MOS6502::readResetVector(void) {
	mProgramCounter = ((*mMemory)[0xFFFD] << 8) | (*mMemory)[0xFFFC];
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
	Byte data = (*mMemory)[0x0100 + (++mStackPointer)];
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
