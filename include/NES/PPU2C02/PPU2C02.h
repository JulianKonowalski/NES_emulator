#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <functional>

#include "IO/Window.h"
#include "NES/Buses/PPUBus.h"
#include "NES/PPU2C02/ColourLUT.h"

enum RENDER_STAGE {
    FETCH_NT = 0,
    FETCH_AT = 2,
    FETCH_TILE_LSB = 4,
    FETCH_TILE_MSB = 6,
    INC_V = 7
};

enum PPU_REGISTER {
    PPUCTRL,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROLL,
    PPUADDR,
    PPUDATA,
    OAMDMA
};

enum CTRL_REGISTER {
    NTADDR1 = 1,        //LSB of base nametable address
    NTADDR2 = 1 << 1,   //MSB of base nametable address
    VRAMINC = 1 << 2,   //address increment per CPU read/write of PPUDATA
    SPRADDR = 1 << 3,   //sprite pattern table address
    BPTADDR = 1 << 4,   //background pattern table address
    SPRTSIZ = 1 << 5,   //sprite size
    PPUMSTR = 1 << 6,   //PPU master/slave select
    VBNMIEN = 1 << 7    //Vblank NMI enable
};

enum MASK_REGISTER {
    RENDER_BACKGROUND = 1 << 3,
    RENDER_SPRITES = 1 << 4
};

enum STATUS_REGISTER {
    SPRITE_OVERFLOW = 1 << 5,
    SPRITE_0 = 1 << 6,
    VBLANK = 1 << 7
};

enum VRAM_MASK {
    COARSE_X = 0b11111,
    COARSE_Y = 0b1111100000,
    NT_SWITCH = 0b110000000000,
    NT_Y = 1 << 11,
    NT_X = 1 << 10,
    FINE_Y = 0b0111000000000000
};

class PPU2C02 {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    PPU2C02(std::function<void(void)> nmiCallback);

    void boot(PPUBus& bus, Window& window);

    void clock(void);

    Byte readRegister(const Word& address);
    void writeRegister(const Byte& data, const Word& address);

private:

    Byte fetchNametable(void);
    Byte fetchAttribute(void);
    Byte fetchTileData(const bool& fetchMsb);

    void updateDataRegisters(void);
    void preRenderRoutine(void);
    void postRenderRoutine(void);
    void incrementX(void);
    void incrementY(void);
    void resetX(void);
    void resetY(void);

    const ColourLUT mColours;

    std::function<void(void)> mNmiCallback; //non-maskable interrupt callback

    PPUBus* mBus;
    Window* mWindow;

    Byte mRegisters[8];     //registers available for CPU write and reads

    Word mVRamAddr;         //vram adderss
    Word mTRamAddr;         //temporary vram address (address of the top left onscreen tile)
    Byte mXScroll;          //fine X scroll

    Word mPShiftReg1;       //1st bitplane of a tile (LSB)
    Word mPShiftReg2;       //2nd bitplane of a tile (MSB)

    Byte mBgTileId;         //fetched tile index
    Byte mBgTileAttribute;  //fetched tile attribute index
    Byte mBgTileLsb;        //fetched tile 1st bitplane
    Byte mBgTileMsb;        //fetched tile 2nd bitplane

    Byte mWLatch;           //1st or 2nd write toggle
    Byte mDataBuffer;       //data fetched from PPUADDR

    short mScanline;        //currently drawn row
    short mCycle;           //currently drawn column
};

#endif // !PPU_H
