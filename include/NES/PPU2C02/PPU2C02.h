#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <functional>

#include "IO/Window.h"
#include "NES/Buses/PPUBus.h"
#include "NES/PPU2C02/ColourLUT.h"

/**
* Stages of rendering
* a single row of a
* background graphic tile.
*/
enum BG_RENDER_STAGE {
    BG_FETCH_NT = 0,
    BG_FETCH_AT = 2,
    BG_FETCH_LSB = 4,
    BG_FETCH_MSB = 6,
    BG_INC_H = 7
};

/**
* Stages of rendering
* a single row of a
* foreground graphic tile.
*/
enum FG_RENDER_STAGE {
    FG_FETCH_Y,
    FG_FETCH_NT,
    FG_FETCH_AT,
    FG_FETCH_X,
    FG_UPDATE_SHIFTER
};

/**
* PPU registers available
* for outside reads and
* writes.
*/
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

/**
* Masks for accessing
* the data stored in
* PPUCTRL register.
*/
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

/**
* Masks for accessing
* the data stored in
* PPUMASK register.
*/
enum MASK_REGISTER {
    RENDER_BACKGROUND = 1 << 3,
    RENDER_SPRITES = 1 << 4
};

/**
* Masks for accessing
* the data stored in
* PPUSTATUS register.
*/
enum STATUS_REGISTER {
    SPRITE_OVERFLOW = 1 << 5,   //sprite overflow flag
    SPRITE_0 = 1 << 6,          //sprite 0 hit flag
    VBLANK = 1 << 7             //vblank flag
};

/**
* Masks for accessing
* the data stored in
* the internal VRAM
* address register.
*/
enum VRAM_MASK {
    COARSE_X = 0b11111,
    COARSE_Y = 0b1111100000,
    NT_SWITCH = 0b110000000000,
    NT_Y = 1 << 11,
    NT_X = 1 << 10,
    FINE_Y = 0b0111000000000000
};

/**
* Masks for accessing
* the sprite parameters 
* stored in sprite attribute 
* data.
*/
enum SPRITE_MASK {
    FLIP_V = 0b10000000,
    FLIP_H = 0b01000000,
    PRIORITY = 0b00100000,
    PALETTE = 0b00000011
};


/**
* Class that emulates the
* behaviour of the NES'
* PPU.
*/
class PPU2C02 {
public:

    using Byte = uint8_t;
    using Word = uint16_t;

    /**
    * Class constructor. Initializes a
    * class instance with given parameters.'
    * 
    * @param nmiCallback non maskable interrupt
    *   callback called at the end of drawing a 
    *   frame.
    * 
    * @see mNmiCallback
    */
    PPU2C02(std::function<void(void)> nmiCallback);

    /**
    * Boots the PPU by assingning a bus and 
    * window to it.
    * 
    * @param bus PPU bus to be connected
    * @param window window for video output
    * 
    * @see PPUBus
    * @see Window
    */
    void boot(PPUBus& bus, Window* window);

    /**
    * Clocks the PPU
    */
    void clock(void);

    /**
    * Reads data from a PPU register
    * under a given address.
    * 
    * @param address address to
    *   read data from
    * 
    * @return data read from the
    *   given address
    */
    Byte readRegister(Word address);

    /**
    * Writes data to a PPU register
    * under a given address.
    * 
    * @param data data to be written
    *   to the register
    * @param address to write the   
    *   data to
    */
    void writeRegister(const Byte& data, Word address);

    /**
    * Starts the dma transfer.
    * Resets the OAMADDR register
    * by setting it to 0x00.
    * 
    * @param data data to be written
    *   into the OAMDATA register
    */
    void startDmaTransfer(const Byte& data);

    /**
    * Writes data to oam memory under
    * OAMADDR address and increments
    * the address stored in OAMADDR
    * register.
    * 
    * @param data data to be written
    *   into OAM memory.
    * 
    * @see mOam
    */
    void writeDma(const Byte& data);

    /**
    * Returns the value of OAMDMA register.
    * 
    * @return data stored in OAMDMA register
    */
    Byte getOamDma(void) { return mRegisters[OAMDMA]; }

    /**
    * Returns the value of OAMADDR register.
    * 
    * @return data stored in OAMADDR register
    */
    Byte getOamAddr(void) { return mRegisters[OAMADDR]; }

private:

    /**
    * Performs all operations
    * liked to rendering frames
    * and updating internal registers.
    */
    void updateState(void);

    /**
    * Evaluates the state of
    * internal registers and
    * draws a pixel based on
    * the outcome.
    */
    void draw(void);
    
    /**
    * Updates the x and y
    * coordinates on the 
    * screen.
    */
    void updatePosition(void);

    /**
    * Fetches the backgroud
    * tile's nametable.
    * 
    * @return background tile's
    *   nametable
    */
    Byte fetchBgNametable(void);

    /**
    * Fetches the backgroud
    * tile's attribute.
    * 
    * @return background tile's
    *   attribute
    */
    Byte fetchBgAttribute(void);

    /**
    * Fetches the background
    * tile's pattern data.
    * 
    * @param fetchMsb flag
    *   indicating if LSB
    *   or MSB of the pattern
    *   data should be fetched
    */
    Byte fetchBgTileData(const bool& fetchMsb);

    /**
    * Fetches the foreground
    * tile's pattern data.
    * 
    * @param fetechMsb flag
    *   indicating if LSB 
    *   or MSB of the pattern
    *   data should be fetched
    */
    Byte fetchFgTileData(const bool& fetchMsb);

    /**
    * Updates internal registers
    * related to rendering the 
    * background based on the 
    * current cycle. Different 
    * operations are performed at 
    * different parts of the scanline.
    */
    void updateBackgroundData(void);

    /**
    * Updates internal registers
    * related to rendering the 
    * foreground.
    */
    void updateSpriteData(void);

    /**
    * Updates all of the internal
    * shift registers by performing
    * a single bitshift.
    */
    void updateShifters(void);

    /**
    * Evaluates OAM memory to
    * determine which sprites 
    * should be rendered.
    */
    void evaluateOam(void);

    /**
    * Checks if the scanline 
    * is rendering the first
    * line of the sprite 0.
    * If the sprite is being
    * rendered the appropriate
    * state flag is set.
    */
    void setSprite0Hit(void);

    /**
    * Performs a prerender
    * routine. All of the 
    * sprite shifters and 
    * status flags are cleared.
    */
    void preRenderRoutine(void);

    /**
    * Performs a postrender
    * routine. VBLANK flag
    * is set and if the VBNMIEN
    * flag is set the NMI callback
    * is called.
    */
    void postRenderRoutine(void);

    /**
    * Increments the X coordinate
    * in the VRAM address.
    * 
    * @see mVramAddr
    */
    void incrementX(void);

    /**
    * Increments the Y coordinate
    * in the VRAM address.
    * 
    * @see mVramAddr
    */
    void incrementY(void);

    /**
    * Resets the X coordinate
    * in the VRAM address.
    * 
    * @see mVramAddr
    */
    void resetX(void);

    /**
    * Resets the Y coordinate
    * in the VRAM address.
    * 
    * @see mVramAddr
    */
    void resetY(void);

    /**
    * Lookup table for the
    * colours supported by
    * the NES' PPU
    */
    const ColourLUT mColours;

    /** Non-maskable interrupt callback */
    std::function<void(void)> mNmiCallback;

    /** Internal PPU bus */
    PPUBus* mBus;

    /** Window for video output */
    Window* mWindow;

    /** 
    * Registers available for 
    * outside reads and writes.
    */
    Byte mRegisters[9];
    
    /** 
    * Object attribute 
    * memory for sprites./
    */
    Byte mOam[256];

    /** 
    * Secondary object 
    * attribute memory. 
    */
    Byte mSecondaryOam[32]; 

    /** 
    * Counter for sprites 
    * on one scanline. 
    */
    Byte mSpriteCount;

    /** 
    * Internal VRAM 
    * address pointer. 
    */
    Word mVRamAddr;

    /** 
    * Temporary VRAM 
    * address pointer.
    */
    Word mTRamAddr;

    /** Fine X scroll value */
    Byte mFineX;

    /** 
    * Fetched backgroud 
    * tile index.
    */
    Byte mBgTileId;         

    /** 
    * Fetched background 
    * tile attribute index.
    */
    Byte mBgTileAttribute;  

    /** 
    * Fetched background 
    * tile 1st bitplane. 
    */
    Byte mBgTileLsb;        

    /** 
    * Fetched background 
    * tile 2nd bitplane.
    */
    Byte mBgTileMsb;        

    /** 
    * 1st bitplane of 
    * a background tile 
    * pattern (LSB).
    */
    Word mBgPatternLo;      

    /** 
    * 2nd bitplane of 
    * a background tile 
    * pattern (MSB).
    */
    Word mBgPatternHi;      

    /** 
    * 1st colour bitplane 
    * of a background tile 
    * (LSB).
    */
    Word mBgAttribLo;       

    /** 
    * 2nd colour bitplane 
    * of a background tile 
    * (MSB).
    */
    Word mBgAttribHi;       

    /** Sprite's y coordinate */
    Byte mFgTileY;

    /** Sprite's tile ID */ 
    Byte mFgTileId;

    /** Sprite's attribute */
    Byte mFgTileAttribute;

    /** Sprite's X coordinate */
    Byte mFgTileX;

    /** 
    * 1st bitplane of 
    * a foreground tile 
    * pattern (LSB).
    */
    Byte mFgPatternLo[8];

    /** 
    * 2nd bitplane of 
    * a foreground tile 
    * pattern (MSB).
    */
    Byte mFgPatternHi[8];

    /**
    * Sprite tiles'
    * pallete codes
    */
    Byte mFgAttrib[8];

    /** Sprites' x coordinates */
    Byte mSpritesXPos[8];

    /** 
    * Latch to indicate 1st 
    * or 2nd write to PPUSCROLL 
    * or PPUADDR.
    */
    Byte mWLatch;           

    /** Data fetched from PPUADDR */
    Byte mDataBuffer;

    /** Currently drawn row */
    short mScanline;        
    
    /** Currently drawn column */
    short mCycle;           
};

#endif // !PPU_H
