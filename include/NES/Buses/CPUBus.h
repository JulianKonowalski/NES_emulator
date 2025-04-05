#ifndef CPUBUS_H
#define CPUBUS_H

#include <cstdint>

#include "NES/PPU2C02/PPU2C02.h"
#include "NES/APU/APU.h"
#include "NES/Cartridge/Cartridge.h"

#include "IO/Joypad.h"

class MOS6502;

/**
* Class emulating the
* behaviour of the main
* NES bus. It connects 
* all of the other components
* and allows for their 
* communication.
* 
* @see MOS6502
* @see APU
* @see PPU
* @see Cartridge
* @see Joypad
*/
class CPUBus {
public:
    using Byte = uint8_t;
    using Word = uint16_t;

    /**
    * Class constructor. Initializes
    * an instance of the class with
    * given parameters.
    * 
    * @param cpu cpu object to be
    *   connected to the bus
    * @param ppu ppu object to be 
    *   connected to the bus
    * @param cartridge cartridge 
    *   object to be connected to
    *   the bus
    * @param joypad joypad object
    *   to be connected to the bus
    * @param globalClock global clock
    *   counter
    * 
    */
    CPUBus(MOS6502& cpu, PPU2C02& ppu, APU& apu, Cartridge& cartridge, Joypad& joypad, Word& globalClock);

    /**
    * Performs a read from
    * the given address.
    * 
    * @param address address to
    *   read from
    * 
    * @return data read from the
    *   given address
    */
    Byte read(const Word& address);

    /**
    * Performs a write to
    * the given address.
    * 
    * @param data data to be
    *   written to the address
    * @param address address to
    *   write the data to
    */
    void write(const Byte& data, const Word& address);

    /**
    * Performs a dma transfer over
    * 512 cpu cycles copying a whole
    * page of CPU RAM to PPU VRAM.
    */
    void dmaTransfer(void);

private:

    /** An array representing the NES' 2KB RAM memory */
    Byte mRam[2048];

    /** CPU component */
    MOS6502* mCpu;

    /** PPU component */
    PPU2C02* mPpu;

    /** APU component */
    APU* mApu;

    /** Cartridge component */
    Cartridge* mCartridge;

    /** Joypad component */
    Joypad* mJoypad;

    /**
    * A flag to determine
    * if the bus should 
    * wait one cycle before
    * starting the DMA procedure.
    */
    bool mDmaWait;

    /**
    * Data to be transferred
    * between the CPU RAM and 
    * PPU VRAM during the DMA
    * transfer.
    */
    Byte mDmaData;

    /** Global clock counter */
    Word* mGlobalClock;
};

#endif // !CPUBUS_H
