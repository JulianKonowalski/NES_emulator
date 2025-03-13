#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <functional>

#include "IO/Window.h"
#include "NES/Buses/PPUBus.h"
#include "NES/PPU2C02/ColourLUT.h"

enum BG_RENDER_STAGE {
    BG_FETCH_NT = 0,
    BG_FETCH_AT = 2,
    BG_FETCH_LSB = 4,
    BG_FETCH_MSB = 6,
    BG_INC_H = 7
};

enum FG_RENDER_STAGE {
    FG_FETCH_Y,
    FG_FETCH_NT,
    FG_FETCH_AT,
    FG_FETCH_X,
    FG_UPDATE_SHIFTER
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
    SPRITE_OVERFLOW = 1 << 5,   //sprite overflow flag
    SPRITE_0 = 1 << 6,          //sprite 0 hit flag
    VBLANK = 1 << 7             //vblank flag
};

enum VRAM_MASK {
    COARSE_X = 0b11111,
    COARSE_Y = 0b1111100000,
    NT_SWITCH = 0b110000000000,
    NT_Y = 1 << 11,
    NT_X = 1 << 10,
    FINE_Y = 0b0111000000000000
};

enum SPRITE_MASK {
    FLIP_V = 0b10000000,
    FLIP_H = 0b01000000,
    PRIORITY = 0b00100000,
    PALETTE = 0b00000011
};

class PPU2C02 {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    PPU2C02(std::function<void(void)> nmiCallback);

    void boot(PPUBus& bus, Window* window);

    void clock(void);

    Byte readRegister(Word address);
    void writeRegister(const Byte& data, Word address);

    void startDmaTransfer(const Byte& data);
    void writeDma(const Byte& data);
    Byte getOamDma(void) { return mRegisters[OAMDMA]; }
    Byte getOamAddr(void) { return mRegisters[OAMADDR]; }

private:

    void updateState(void);
    void draw(void);
    void updatePosition(void);

    Byte fetchBgNametable(void);
    Byte fetchBgAttribute(void);
    Byte fetchBgTileData(const bool& fetchMsb);

    Byte fetchFgTileData(const bool& fetchMsb);

    void updateBackgroundData(void);
    void updateSpriteData(void);
    void updateShifters(void);
    void evaluateOam(void);
    void setSprite0Hit(void);
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

    Byte mRegisters[9];     //registers available for CPU write and reads
    Byte mOam[256];         //internal object attribute memory (for sprites)
    Byte mSecondaryOam[32]; //secondary object attribute memory
    Byte mSpriteCount;

    Word mVRamAddr;         //vram adderss
    Word mTRamAddr;         //temporary vram address (address of the top left onscreen tile)
    Byte mFineX;            //fine X scroll

    Byte mBgTileId;         //fetched background tile index
    Byte mBgTileAttribute;  //fetched background tile attribute index
    Byte mBgTileLsb;        //fetched background tile 1st bitplane
    Byte mBgTileMsb;        //fetched background tile 2nd bitplane

    Word mBgPatternLo;      //1st bitplane of a tile (LSB)
    Word mBgPatternHi;      //2nd bitplane of a tile (MSB)
    Word mBgAttribLo;       //1st colour bitplane of a tile (LSB)
    Word mBgAttribHi;       //2nd colour bitplane of a tile (MSB)

    Byte mFgTileY;
    Byte mFgTileId;
    Byte mFgTileAttribute;
    Byte mFgTileX;

    Byte mFgPatternLo[8];
    Byte mFgPatternHi[8];
    Byte mFgAttrib[8];
    Byte mSpritesXPos[8];

    Byte mWLatch;           //1st or 2nd write toggle
    Byte mDataBuffer;       //data fetched from PPUADDR

    short mScanline;        //currently drawn row
    short mCycle;           //currently drawn column
};

#endif // !PPU_H
