#ifndef MEM_INIT
#define MEM_INIT

#include <Memory/Memory.h>

void memInit(Memory* memory) {
	(*memory)[0xFFFD] = 0x12;	//reset vector
	(*memory)[0xFFFC] = 0x00;	//reset vector
}

#endif // !MEM_INIT
