#include <MOS6502/AddressingMode.h>

#include <MOS6502/MOS6502.h>

using Byte = AddressingMode::Byte;
using Word = AddressingMode::Word;

bool AddressingMode::mPageCrossed = false;
void AddressingMode::addCycles(MOS6502& cpu, const unsigned short& cycles) { cpu.addCycles(cycles); }
void AddressingMode::setFetched(MOS6502& cpu, const Byte& data) { cpu.setFetched(data); }
Byte AddressingMode::fetchByte(MOS6502& cpu) { return cpu.fetchByte(); }
Byte AddressingMode::fetchByte(MOS6502& cpu, const Word& address) { return cpu.fetchByte(address); }
Word AddressingMode::fetchFromProgramCounter(MOS6502& cpu) { return cpu.mProgramCounter++; } //this is a special case, when the address of data to be loaded is pointed to by the program counter, used only in IMM addressing

UndefinedAddressingMode* UndefinedAddressingMode::sInstance = nullptr;
UndefinedAddressingMode* UndefinedAddressingMode::getInstance(void) {
	if (!UndefinedAddressingMode::sInstance) { UndefinedAddressingMode::sInstance = new UndefinedAddressingMode(); }
	return UndefinedAddressingMode::sInstance;
}
Word UndefinedAddressingMode::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	return 0;
}

ACC* ACC::sInstance = nullptr;
ACC* ACC::getInstance(void) {
	if (!ACC::sInstance) { ACC::sInstance = new ACC(); }
	return ACC::sInstance;
}
Word ACC::getAddress(MOS6502& cpu) { 
	AddressingMode::mPageCrossed = false;
	return 0;
}

IMP* IMP::sInstance = nullptr;
IMP* IMP::getInstance(void) {
	if (!IMP::sInstance) { IMP::sInstance = new IMP(); }
	return IMP::sInstance;
}
Word IMP::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	return 0;
}

IMM* IMM::sInstance = nullptr;
IMM* IMM::getInstance(void) {
	if (!IMM::sInstance) { IMM::sInstance = new IMM(); }
	return IMM::sInstance;
}
Word IMM::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	return this->fetchFromProgramCounter(cpu);
}

ZP0* ZP0::sInstance = nullptr;
ZP0* ZP0::getInstance(void) {
	if (!ZP0::sInstance) { ZP0::sInstance = new ZP0(); }
	return ZP0::sInstance;
}
Word ZP0::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	return 0 | this->fetchByte(cpu);
}

ZPX* ZPX::sInstance = nullptr;
ZPX* ZPX::getInstance(void) {
	if (!ZPX::sInstance) { ZPX::sInstance = new ZPX(); }
	return ZPX::sInstance;
}
Word ZPX::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	return 0 | Byte(this->fetchByte(cpu) + cpu.getX());
}

ZPY* ZPY::sInstance = nullptr;
ZPY* ZPY::getInstance(void) {
	if (!ZPY::sInstance) { ZPY::sInstance = new ZPY(); }
	return ZPY::sInstance;
}
Word ZPY::getAddress(MOS6502& cpu) { 
	AddressingMode::mPageCrossed = false;
	return 0 | Byte(this->fetchByte(cpu) + cpu.getY());
}

REL* REL::sInstance = nullptr;
REL* REL::getInstance(void) {
	if (!REL::sInstance) { REL::sInstance = new REL(); }
	return REL::sInstance;
}
Word REL::getAddress(MOS6502& cpu) { 
	AddressingMode::mPageCrossed = false;
	signed char offset = this->fetchByte(cpu); 
	if (offset < 0) { ++offset; } //additional offset for negative numbers
	return cpu.getProgramCounter() + offset;
}

ABS* ABS::sInstance = nullptr;
ABS* ABS::getInstance(void) {
	if (!ABS::sInstance) { ABS::sInstance = new ABS(); }
	return ABS::sInstance;
}
Word ABS::getAddress(MOS6502& cpu) { 
	AddressingMode::mPageCrossed = false;
	Byte low = this->fetchByte(cpu);
	return (this->fetchByte(cpu) << 8) | low;
}

ABX* ABX::sInstance = nullptr;
ABX* ABX::getInstance(void) {
	if (!ABX::sInstance) { ABX::sInstance = new ABX(); }
	return ABX::sInstance;
}
Word ABX::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	Byte low = this->fetchByte(cpu);
	Word high = this->fetchByte(cpu) << 8;
	Word address = (high | low) + cpu.getX();
	if (Byte(address >> 8) != Byte(high >> 8)) { AddressingMode::mPageCrossed = true; }
	return address;
}

ABY* ABY::sInstance = nullptr;
ABY* ABY::getInstance(void) {
	if (!ABY::sInstance) { ABY::sInstance = new ABY(); }
	return ABY::sInstance;
}
Word ABY::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	Byte low = this->fetchByte(cpu);
	Word high = this->fetchByte(cpu) << 8;
	Word address = (high | low) + cpu.getY();
	if (Byte(address >> 8) != Byte(high >> 8)) { AddressingMode::mPageCrossed = true; }
	return address;
}

IND* IND::sInstance = nullptr;
IND* IND::getInstance(void) {
	if (!IND::sInstance) { IND::sInstance = new IND(); }
	return IND::sInstance;
}
Word IND::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	Byte lowIndirect = this->fetchByte(cpu);
	Word highIndirect = this->fetchByte(cpu) << 8;
	Word lowDirect = highIndirect | lowIndirect;
	Word highDirect = highIndirect | Byte(lowIndirect + 1); //this is a known bug -- when jumping to 0x**FF, the CPU will read 0x**FF and 0x**00
	return (this->fetchByte(cpu, highDirect) << 8) | this->fetchByte(cpu, lowDirect);
}

IDX* IDX::sInstance = nullptr;
IDX* IDX::getInstance(void) {
	if (!IDX::sInstance) { IDX::sInstance = new IDX(); }
	return IDX::sInstance;
}
Word IDX::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	Byte zpAddress = this->fetchByte(cpu) + cpu.getX();
	Byte lowDirect = this->fetchByte(cpu, zpAddress);
	return (this->fetchByte(cpu, Byte(zpAddress + 1)) << 8) | lowDirect;
}

IDY* IDY::sInstance = nullptr;
IDY* IDY::getInstance(void) {
	if (!IDY::sInstance) { IDY::sInstance = new IDY(); }
	return IDY::sInstance;
}
Word IDY::getAddress(MOS6502& cpu) {
	AddressingMode::mPageCrossed = false;
	Byte zpAddress = this->fetchByte(cpu);
	Word lowDirect = this->fetchByte(cpu, zpAddress);
	Word highDirect = this->fetchByte(cpu, Byte(zpAddress + 1)); //same overflow bug
	Word address = ((highDirect << 8) | lowDirect) + cpu.getY();
	if (Byte(address >> 8) != highDirect) { AddressingMode::mPageCrossed = true; }
	return address;
}
