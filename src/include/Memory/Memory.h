#ifndef MEMORY_H
#define MEMORY_H

#include <cstring>

const size_t kMaxMemorySize = 1024 * 64;

class Memory {

	using Byte = unsigned char;
	using Word = unsigned short;

public:

	Memory(void) { this->clear(); }
	void clear(void) { memset(mData, 0, kMaxMemorySize); }
	Byte& operator[](const size_t& index) { return mData[index]; }

private:
	Byte mData[kMaxMemorySize];
};

#endif // !MEMORY_H
