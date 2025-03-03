#include "NES/MOS6502/MOS6502.h"

using Byte = MOS6502::Byte;

MOS6502::MOS6502() : 
	mCycles(8),
	mAccAddressing(false),
	mFetchedAddress(0),
	mProgramCounter(0),
	mStackPointer(0),
	mAccumulator(0),
	mX(0),
	mY(0),
	mStatusRegister(ProcessorFlag::FLAG_DEFAULT), //this bit is always set to 1
	mBus(nullptr)
{};

void MOS6502::boot(CPUBus& bus) {
	mBus = &bus;
	for (int i = 0; i < 3; ++i) { this->pushStack(0); } //these are 3 fake pushes described in the docs
	this->readResetVector();
}

void MOS6502::clock(void) { //execute only when cycle count == 0
	if (!mCycles) { this->executeInstruction(); }
	--mCycles;
}

void MOS6502::nmi(void) {
	this->pushStack(mProgramCounter >> 8);
	this->pushStack((Byte)mProgramCounter);
	this->pushStack(mStatusRegister);
	this->readNmiVector();
}

void MOS6502::readResetVector(void) {
	mProgramCounter = mBus->read(0xFFFD) << 8 | mBus->read(0xFFFC);
}

void MOS6502::readNmiVector(void) {
	mProgramCounter = mBus->read(0xFFFB) << 8 | mBus->read(0xFFFA);
}

void MOS6502::executeInstruction(void) {
	Byte opcode = mBus->read(mProgramCounter++);
	mOpcodes[opcode].execute(*this);
}


/* MEMORY OPERATIONS */


Byte MOS6502::fetchByte(void) { return mBus->read(mProgramCounter++); }
Byte MOS6502::fetchByte(const Word& address) { return mBus->read(address); }

Byte MOS6502::fetchStack(void) {
	Byte data = mBus->read(0x100 + (++mStackPointer));	//stkptr will be incremented before the read
	return data;
}
void MOS6502::pushStack(const Byte& data) { 
	mBus->write(data, 0x100 + mStackPointer--);			//stkptr will be decremented after the write
}

void MOS6502::writeMemory(const Byte& data, const Word& address) { 
	mBus->write(data, address);
}


/* FLAG OPERATIONS */


void MOS6502::setFlag(const ProcessorFlag& flag, const bool& state) {
	if (state) { mStatusRegister |= flag; } 
	else { mStatusRegister &= ~flag; }
}
