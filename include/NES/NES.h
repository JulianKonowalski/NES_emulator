#ifndef NES_H
#define NES_H

#include "NES/MOS6502/MOS6502.h"
#include "NES/PPU2C02/PPU2C02.h"
#include "NES/Buses/CPUBus.h"
#include "NES/Buses/PPUBus.h"
#include "NES/APU/APU.h"
#include "NES/Cartridge/Cartridge.h"

#include "IO/Window.h"
#include "IO/Joypad.h"

/**
* Class that emulates the
* behaviour of the NES (
* Nintendo Entertainment
* System).
*/
class NES {
public:

	using Byte = unsigned char;
	using Word = unsigned short;

	/**
	* Class constructor. It initializes
	* an instance of the class with
	* given parameters. It creates
	* all of the necessary components
	* and starts an application window.
	* 
	* @param cartridge cartridge object
	*	containing the iNES file data
	*/
	NES(Cartridge& cartridge);

	/**
	* Class destructor. It destroys
	* the created application window.
	*/
	~NES(void);

	/**
	* Starts the main app loop.
	*/
	void run(void);

private:

	/** Clock counter */
	Word mClock;

	/** Application window */
	Window* mWindow;

	/** Audio Processing Unit */
	APU mApu;

	/** Central Processing Unit */
	MOS6502 mCpu;

	/** Picture Processing Unit */
	PPU2C02 mPpu;

	/** Main system bus */
	CPUBus mCpuBus;

	/** Internal PPU bus */
	PPUBus mPpuBus;

	/** Joypads */
	Joypad mJoypads[2];
};

#endif // !NES_H
