#include "NES/PPU2C02/PPU2C02.h"

using Byte = PPU2C02::Byte;
using Word = PPU2C02::Word;

PPU2C02::PPU2C02(std::function<void(void)> nmiCallback) :
    mNmiCallback(nmiCallback),
    mWindow(nullptr),
    mBus(nullptr),
    mScanline(-1),
    mCycle(0),
    mPShiftReg1(0),
    mPShiftReg2(0),
    mBgTileId(0),
    mBgTileAttribute(0),
    mBgTileLsb(0),
    mBgTileMsb(0),
    mVRamAddr(0),
    mTRamAddr(0),
    mXScroll(0),
    mWLatch(0),
    mDataBuffer(0)
{
    for (int i = 0; i < 8; ++i) { mRegisters[i] = 0; }
}

void PPU2C02::boot(PPUBus& bus, Window& window) {
    mBus = &bus;
    mWindow = &window;
}

void PPU2C02::clock(void) {

    //update PPU state
    if (mScanline < 240) {       //visible + prerender scanline

        if (mScanline == -1) {   //prerender scanline
            if (mCycle == 0) { this->preRenderRoutine(); }
            else if (mCycle >= 279 && mCycle < 304) { this->resetY(); }
        }

        if (mCycle < 0) { /* DO NOTHING */ }           //prerender cycle
        else if (mCycle < 256 || (mCycle >= 320 && mCycle < 336)) { this->updateDataRegisters(); } //visible

        if (mCycle == 255) { this->incrementY(); }     //end of scanline
        else if (mCycle == 256) { this->resetX(); }    //end of scanline

    }
    else if (mScanline == 240) { /* DO NOTHING */ }                             //postrender scanline
    else if (mScanline == 241 && mCycle == 0) { this->postRenderRoutine(); }    //start of vblank

    //draw
    Byte shift = 15 - mXScroll;
    Byte pixelCode = ((mPShiftReg1 & (1 << shift)) >> shift) + ((mPShiftReg2 & (1 << shift)) >> shift);

    //mBgTileAttribute = 0; //FIX THE COLOURS

    Byte colourCode = mBus->read(0x3F00 | (mBgTileAttribute << 2) | pixelCode);
    mWindow->drawPixel(mCycle, mScanline, mColours[colourCode]);

    //update screen position
    if (++mCycle > 340) {           //341 cycles per scanline
        mCycle = -1;
        if (++mScanline > 261) {    //262 scanlines
            mScanline = -1;
            mWindow->swapBuffers();
        } 
    }

}

Byte PPU2C02::readRegister(const Word& address) {
    Byte operation = address & 0x7;
    if (operation == PPUCTRL
        || operation == PPUMASK
        || operation == OAMADDR
        || operation == PPUADDR
        || operation == PPUSCROLL
    ) { return 0; } //all are write only

    Byte data = 0;  //helper variable
    switch (operation) {
    case PPUSTATUS:
        data = mRegisters[PPUSTATUS];
        mRegisters[PPUSTATUS] &= ~STATUS_REGISTER::VBLANK;
        mWLatch = 0;
        return data;
    case OAMDATA: 
        /* TODO */ 
        return 0;
    case PPUDATA:
        if (mVRamAddr >= 0x3F00) {  //no delay for palette reads 
            mDataBuffer = mBus->read(mVRamAddr);
            ++mVRamAddr; 
            return mDataBuffer;
        }
        data = mDataBuffer;
        mDataBuffer = mBus->read(mVRamAddr);
        mVRamAddr += mRegisters[PPUCTRL] & CTRL_REGISTER::VRAMINC ? 32 : 1;
        return data;
    }
}

void PPU2C02::writeRegister(const Byte& data, const Word& address) {
    switch (address &0x7) {
    case PPUCTRL: 
        mRegisters[PPUCTRL] = data;
        mTRamAddr = (mTRamAddr & ~VRAM_MASK::NT_SWITCH) | ((data & 0x3) << 10);
        break;
    case PPUMASK: mRegisters[PPUMASK] = data; break;
    case PPUSTATUS: break; //read only
    case OAMADDR: mRegisters[OAMADDR] = data; break;
    case OAMDATA: 
        /* TODO */ 
        break;
    case PPUSCROLL:
        if (!mWLatch) {
            mTRamAddr = (mTRamAddr & ~VRAM_MASK::COARSE_X) | (data >> 3);
            mXScroll = data & 0x7;
            mWLatch = 1;
        } else {
            mTRamAddr = (
                (mTRamAddr & ~(VRAM_MASK::COARSE_Y | VRAM_MASK::FINE_Y)) | 
                ((data & 0b111) << 12) | 
                ((data >> 3) << 5)
            );
            mWLatch = 0;
        } 
        break;
    case PPUADDR:
        if (!mWLatch) {
            mTRamAddr = (mTRamAddr & 0x00FF) | (data & 0x3F) << 8;
            mWLatch = 1;
        } else {
            mTRamAddr = (mTRamAddr & 0xFF00) | data;
            mVRamAddr = mTRamAddr;
            mWLatch = 0;
        }
        break;
    case PPUDATA:
        mBus->write(data, mVRamAddr);
        mVRamAddr += mRegisters[PPUCTRL] & CTRL_REGISTER::VRAMINC ? 32 : 1;
        break;
    }
}

Byte PPU2C02::fetchNametable(void) {
    return mBus->read(
        0x2000                              //base address
        + (mVRamAddr & VRAM_MASK::COARSE_X) //coarse X offset
        + (mVRamAddr & VRAM_MASK::COARSE_Y) //coarse Y offset
    );
}

Byte PPU2C02::fetchAttribute(void) {
    Byte attribute = mBus->read(
        0x23C0                                              //base attribute table address
        | (mVRamAddr & VRAM_MASK::NT_SWITCH)                //nametable select
        | (((mVRamAddr & VRAM_MASK::COARSE_Y) >> 7) << 3)   //coarse Y
        | ((mVRamAddr & VRAM_MASK::COARSE_X) >> 2)          //coarse X
    );
    if (((mVRamAddr & VRAM_MASK::COARSE_Y) >> 5) & 0x2) { attribute >>= 4; }
    if ((mVRamAddr & VRAM_MASK::COARSE_X) & 0x2) { attribute >>= 2; }
    attribute &= 0x3;
    return attribute;

}

Byte PPU2C02::fetchTileData(const bool& fetchMsb) {
    return mBus->read(
        (mRegisters[PPUCTRL] & CTRL_REGISTER::BPTADDR ? 0x1000 : 0) //background address offset
        + (mBgTileId << 4)                                          //tileId * tileSize(16)
        + ((mVRamAddr & VRAM_MASK::FINE_Y) >> 12)                   //fine Y offset
        + (fetchMsb ? 8 : 0)                                        //MSB offset
    );
}

void PPU2C02::updateDataRegisters(void) {

    mPShiftReg1 <<= 1;
    mPShiftReg2 <<= 1;

    switch (mCycle % 8) {
    case RENDER_STAGE::FETCH_NT:
        mPShiftReg1 |= mBgTileLsb;
        mPShiftReg2 |= mBgTileMsb;
        mBgTileId = this->fetchNametable(); 
        break;
    case RENDER_STAGE::FETCH_AT: mBgTileAttribute = this->fetchAttribute(); break;
    case RENDER_STAGE::FETCH_TILE_LSB: mBgTileLsb = this->fetchTileData(false); break;
    case RENDER_STAGE::FETCH_TILE_MSB: mBgTileMsb = this->fetchTileData(true); break;
    case RENDER_STAGE::INC_V: this->incrementX(); break;
    }
}

void PPU2C02::preRenderRoutine(void) {
    mRegisters[PPUSTATUS] &= ~STATUS_REGISTER::VBLANK;
    mRegisters[PPUSTATUS] &= ~STATUS_REGISTER::SPRITE_0;
    mRegisters[PPUSTATUS] &= ~STATUS_REGISTER::SPRITE_OVERFLOW;
}

void PPU2C02::postRenderRoutine(void) {
    mRegisters[PPUSTATUS] |= STATUS_REGISTER::VBLANK;
    if (mRegisters[PPUCTRL] & CTRL_REGISTER::VBNMIEN) { this->mNmiCallback(); }
}

void PPU2C02::incrementX(void) {
    if (
        (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES)
    ) {
        Byte X = mVRamAddr & VRAM_MASK::COARSE_X;   
        if (X < 31) {
            mVRamAddr = (mVRamAddr & ~VRAM_MASK::COARSE_X) | ++X;
            return;
        }
        mVRamAddr = (
            (mVRamAddr & ~(VRAM_MASK::NT_X | VRAM_MASK::COARSE_X))
            | (~(mVRamAddr & VRAM_MASK::NT_X) & VRAM_MASK::NT_X)
        );
    }
}

void PPU2C02::incrementY(void) {
    if (
        (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES)
    ) {
        Byte fineY = (mVRamAddr & VRAM_MASK::FINE_Y) >> 12;
        if (fineY < 7) {
            mVRamAddr = (mVRamAddr & ~VRAM_MASK::FINE_Y) | (++fineY << 12);
            return;
        }
        Byte coarseY = (mVRamAddr & VRAM_MASK::COARSE_Y) >> 5;
        if (coarseY < 29) {
            mVRamAddr = (mVRamAddr & ~(VRAM_MASK::FINE_Y | VRAM_MASK::COARSE_Y)) | (++coarseY << 5);
            return;
        }
        mVRamAddr = (
            (mVRamAddr & ~(VRAM_MASK::FINE_Y | VRAM_MASK::NT_Y | VRAM_MASK::COARSE_Y) 
            | (~(mVRamAddr & VRAM_MASK::NT_Y) & VRAM_MASK::NT_Y))
        );
    }
}

void PPU2C02::resetX(void) {
    if (
        (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES)
    ) {
        Word mask = VRAM_MASK::COARSE_X | VRAM_MASK::NT_X;
        mVRamAddr = (
            (mVRamAddr & ~mask)
            | (mTRamAddr & mask)
        );
    }
}

void PPU2C02::resetY(void) {
    if (
        (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES)
    ) {
        Word mask = VRAM_MASK::COARSE_Y | VRAM_MASK::FINE_Y | VRAM_MASK::NT_Y;
        mVRamAddr = (
            (mVRamAddr & ~mask)
            | (mTRamAddr & mask)
        );
    }
}
