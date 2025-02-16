#include "NES/Nes.h"

#include <fstream>

NES::NES(const std::string& programPath) {
	try {
		this->loadProgram(programPath);
		mCPU.boot(&mMemory); 
	} catch (const std::runtime_error& error) {
		std::cout << error.what();
		exit(0);
	}
}

void NES::run(void) {
	while (true) {  mCPU.clock(); }
}

void NES::loadProgram(const std::string programPath) {
	std::ifstream programFile(programPath, std::ios_base::binary);
	int i = 0;
	while (!programFile.eof()) {
		if (!programFile.good()) {
			throw std::runtime_error(std::string("Error: Failed to load " + programPath));
		} 
		char tmp;
		programFile.read(&tmp, 1);
		if (i >= kMaxMemorySize && !programFile.eof()) {
			throw std::runtime_error("Error: Out of memory");
		}
		mMemory[i] = Byte(tmp);
		++i;
	}
	programFile.close();
}