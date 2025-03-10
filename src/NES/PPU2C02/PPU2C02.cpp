#include "NES/PPU2C02/PPU2C02.h"

#include <cstdlib>

using Byte = PPU2C02::Byte;
using Word = PPU2C02::Word;

PPU2C02::PPU2C02(std::function<void(void)> nmiCallback) :
    mNmiCallback(nmiCallback),
    mBus(nullptr),
    mWindow(nullptr),
    mSpriteCount(0),
    mVRamAddr(0),
    mTRamAddr(0),
    mFineX(0),
    mBgTileId(0),
    mBgTileAttribute(0),
    mBgTileLsb(0),
    mBgTileMsb(0),
    mBgPatternLo(0),
    mBgPatternHi(0),
    mBgAttribLo(0),
    mBgAttribHi(0),
    mFgTileY(0),
    mFgTileId(0),
    mFgTileAttribute(0),
    mFgTileX(0),
    mWLatch(0),
    mDataBuffer(0),
    mScanline(-1),
    mCycle(-1)
{
    memset(mRegisters, 0, 8);
    memset(mOam, 0, 256);
    memset(mSecondaryOam, 0, 32);
    memset(mFgPatternLo, 0, 8);
    memset(mFgPatternHi, 0, 8);
    memset(mFgAttrib, 0, 8);
    memset(mSpritesXPos, 0, 8);
}

void PPU2C02::boot(PPUBus& bus, Window& window) {
    mBus = &bus;
    mWindow = &window;
}

void PPU2C02::clock(void) {
    this->updateState();
    this->draw();
    this->updatePosition();
    if (mScanline == -1 && mCycle == -1)
        mWindow->swapBuffers();
}

Byte PPU2C02::readRegister(Word address) {

    address &= 0x7;

    if(address == PPUCTRL 
        || address == PPUMASK
        || address == OAMADDR
        || address == PPUSCROLL
        || address == PPUADDR
    ) { return 0; } //all are write only

    Byte data; //helper variable
    switch (address) {
        case PPUSTATUS:
            data = mRegisters[PPUSTATUS];
            mRegisters[PPUSTATUS] &= ~(VBLANK);
            mWLatch = 0;
            return data;
        case OAMDATA:
            return mOam[mRegisters[OAMADDR]];
        case PPUDATA:
            data = mDataBuffer;
            mDataBuffer = mBus->read(mVRamAddr);
            if (mVRamAddr >= 0x3F00)
                data = mDataBuffer; //no delay for palette reads
            mVRamAddr += mRegisters[PPUCTRL] & VRAMINC ? 32 : 1;
            return data;
    }
}

void PPU2C02::writeRegister(const Byte& data, Word address) {

    address &= 0x7;

    if (address == PPUSTATUS)
        return; //read only
    
    switch (address) {
        case PPUCTRL:
            mRegisters[PPUCTRL] = data;
            mTRamAddr = (
                (mTRamAddr & ~(NT_SWITCH)) 
                | ((data & 0b11) << 10)
            );
            break;
        case PPUMASK:
            mRegisters[PPUMASK] = data;
            break;
        case OAMADDR:
            mRegisters[OAMADDR] = data;
            break;
        case OAMDATA:
            mOam[mRegisters[OAMADDR]] = data;
            ++mRegisters[OAMADDR];
            break;
        case PPUSCROLL:
            if (!mWLatch) {
                mWLatch = 1;
                mFineX = data & 0b111;
                mTRamAddr = (
                    (mTRamAddr & ~(COARSE_X))
                    | (data >> 3)
                );
            } else {
                mWLatch = 0;
                mTRamAddr = (
                    (mTRamAddr & ~(FINE_Y | COARSE_Y))
                    | ((data & 0b111) << 12)
                    | ((data >> 3) << 5)
                );
            }
            break;
        case PPUADDR:
            if (!mWLatch) {
                mWLatch = 1;
                mTRamAddr = (
                    (mTRamAddr & 0x00FF) 
                    | ((data & 0x3F) << 8)
                );
            } else {
                mWLatch = 0;
                mTRamAddr = (mTRamAddr & 0xFF00) | data;
                mVRamAddr = mTRamAddr;
            }
            break;
        case PPUDATA:
            mBus->write(data, mVRamAddr);
            mVRamAddr += mRegisters[PPUCTRL] & VRAMINC ? 32 : 1;
            break;
    }
}

void PPU2C02::startDmaTransfer(const Byte& data) {
    mRegisters[OAMDMA] = data;
    mRegisters[OAMADDR] = 0x00;
}

void PPU2C02::writeDma(const Byte& data) {
    mOam[mRegisters[OAMADDR]] = data;
    ++mRegisters[OAMADDR];
}

void PPU2C02::updateState(void) {

    if (mCycle < 0) //prerender cycle
        return;

    if (mScanline < 240) {      //visible + prerender scanline

        if (mScanline == -1) {  //specific prerender scanline behaviour
            if (mCycle == 0)    //the rest of the scanline is standard
                this->preRenderRoutine();
            else if (mCycle >= 279 && mCycle < 304)
                this->resetY();
        }

        if (mCycle < 256
            || (mCycle >= 320 && mCycle < 336)) {
            this->updateShifters();
            this->updateBackgroundData();
        }

        if (mCycle == 255) 
            this->incrementY();

        if (mCycle == 256)
            this->resetX();

        if (mCycle >= 256 && mCycle < 320)
            mRegisters[OAMADDR] = 0x00;     //reset OAMADDR
        
        if (mCycle == 339 && mScanline >= 0) { //update sprite data on the last cycle
            memset(mSecondaryOam, 0xFF, 32); //clear secondary OAM
            mSpriteCount = 0;
            this->evaluateOam();
            this->updateSpriteData();
        }

    }
    else if (mScanline == 241 && mCycle == 0) 
        this->postRenderRoutine();
}

void PPU2C02::draw(void) {

    Byte bgPixelCode = 0;
    Byte bgPaletteCode = 0;

    if (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND) {
        Byte shift = 15 - mFineX;
        bgPixelCode = (
            ((mBgPatternLo & (1 << shift)) >> shift)         //low byte
            | ((mBgPatternHi & (1 << shift)) >> (shift - 1)) //high byte
        );
        bgPaletteCode = (
            ((mBgAttribLo & (1 << shift)) >> shift)         //low byte
            | ((mBgAttribHi & (1 << shift)) >> (shift - 1)) //high byte
        );
    }

    Byte fgPixelCode = 0;
    Byte fgPaletteCode = 0;
    Byte fgPriority = 0;

    if (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES) {
        for (int i = 0; i < 8; ++i) {
            if (mSpritesXPos[i] != 0) { continue; } //sprite not visible
            fgPixelCode = ((mFgPatternHi[i] >> 7) << 1) | (mFgPatternLo[i] >> 7);
            fgPaletteCode = (mFgAttrib[i] & SPRITE_MASK::PALETTE) + 0x04;
            fgPriority = (mFgAttrib[i] & SPRITE_MASK::PRIORITY) == 0;  //to test the fg against the bg
            if (fgPixelCode != 0) { 
                if (!i) { this->setSprite0Hit(); }
                break; 
            } //found sprite with highest priority
        }
    }

    Byte pixelCode = 0;
    Byte paletteCode = 0;

    if (bgPixelCode == 0 && fgPixelCode == 0) {
        pixelCode = 0;
        paletteCode = 0;
    } else if (bgPixelCode == 0 && fgPixelCode > 0) {
        pixelCode = fgPixelCode;
        paletteCode = fgPaletteCode;
    } else if (bgPixelCode > 0 && fgPixelCode == 0) {
        pixelCode = bgPixelCode;
        paletteCode = bgPaletteCode;
    } else {
        if (fgPriority) {
            pixelCode = fgPixelCode;
            paletteCode = fgPaletteCode;
        } else {
            pixelCode = bgPixelCode;
            paletteCode = bgPaletteCode;
        }
    }

    Byte colourCode = mBus->read(0x3F00 + (paletteCode << 2) + pixelCode);
    mWindow->drawPixel(mCycle, mScanline, mColours[colourCode]);
}

void PPU2C02::updatePosition(void) {
    if (++mCycle > 339) {           //341 cycles per scanline (-1 through 339)
        mCycle = -1;
        if (++mScanline > 260)      //262 scanlines (-1 through 260)
            mScanline = -1;
    }
}

Byte PPU2C02::fetchBgNametable(void) {
    Word mask = COARSE_X | COARSE_Y | NT_SWITCH;
    return mBus->read(
        0x2000                  //base address
        | (mVRamAddr & mask)    //tile offset
    );
}

Byte PPU2C02::fetchBgAttribute(void) {
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

Byte PPU2C02::fetchBgTileData(const bool& fetchMsb) {
    return mBus->read(
        (mRegisters[PPUCTRL] & CTRL_REGISTER::BPTADDR ? 0x1000 : 0) //background address offset
        + (mBgTileId << 4)                                          //tileId * tileSize(16)
        + ((mVRamAddr & VRAM_MASK::FINE_Y) >> 12)                   //fine Y offset
        + (fetchMsb ? 8 : 0)                                        //MSB offset
    );
}

Byte PPU2C02::fetchFgTileData(const bool& fetchMsb) {

    Word tileDataAddr = 0;

    if (mRegisters[PPUCTRL] & CTRL_REGISTER::SPRTSIZ) { //8x16 mode

        if (mFgTileAttribute & SPRITE_MASK::FLIP_V) {   //flipped vertically
            if ((mScanline - mFgTileY) < 8) {   //top half
                tileDataAddr = (
                    ((mFgTileId & 0x01) << 12)
                    | (((mFgTileId & 0xFE) + 1) << 4)
                    | (7 - ((mScanline - mFgTileY) & 0x07))
                );
            } else {                            //bottom half
                tileDataAddr = (
                    ((mFgTileId & 0x01) << 12)
                    | ((mFgTileId & 0xFE) << 4)
                    | (7 - ((mScanline - mFgTileY) & 0x07))
                );
            }
        } else {    //normal
            if ((mScanline - mFgTileY) < 8) {   //top half
                tileDataAddr = (
                    ((mFgTileId & 0x01) << 12)
                    | ((mFgTileId & 0xFE) << 4)
                    | ((mScanline - mFgTileY) & 0x07)
                );
            } else {                            //bottom half
                tileDataAddr = (
                    ((mFgTileId & 0x01) << 12)
                    | (((mFgTileId & 0xFE) + 1) << 4)
                    | ((mScanline - mFgTileY) & 0x07)
                );               
            }
        }

    } else { //8x8 mode

        if (mFgTileAttribute & SPRITE_MASK::FLIP_V) {   //flipped verically
            tileDataAddr = (
                ((mRegisters[PPUCTRL] & CTRL_REGISTER::SPRADDR) << 9)
                | (mFgTileId << 4)
                | (7 - (mScanline - mFgTileY))
            );
        } else {    //normal
            tileDataAddr = (
                ((mRegisters[PPUCTRL] & CTRL_REGISTER::SPRADDR) << 9)
                | (mFgTileId << 4)
                | (mScanline - mFgTileY)
            );
        }

    }

    tileDataAddr += fetchMsb ? 8 : 0;
    Byte tileData = mBus->read(tileDataAddr);

    if (mFgTileAttribute & SPRITE_MASK::FLIP_H) {
        tileData = (tileData & 0xF0) >> 4 | (tileData & 0x0F) << 4;
        tileData = (tileData & 0xCC) >> 2 | (tileData & 0x33) << 2;
        tileData = (tileData & 0xAA) >> 1 | (tileData & 0x55) << 1;
    }

    return tileData;
}

void PPU2C02::updateBackgroundData(void) {

    switch (mCycle % 8) {
        case BG_FETCH_NT:
            mBgPatternLo |= mBgTileLsb; 
            mBgPatternHi |= mBgTileMsb;
            mBgAttribLo |= mBgTileAttribute & 0b01 ? 0xFF : 0x00;
            mBgAttribHi |= mBgTileAttribute & 0b10 ? 0xFF : 0x00;
            mBgTileId = this->fetchBgNametable();
            break;
        case BG_FETCH_AT: mBgTileAttribute = this->fetchBgAttribute(); break;
        case BG_FETCH_LSB: mBgTileLsb = this->fetchBgTileData(false); break;
        case BG_FETCH_MSB: mBgTileMsb = this->fetchBgTileData(true); break;
        case BG_INC_H: this->incrementX(); break;
    }

}

void PPU2C02::updateSpriteData(void) {

    Byte secondaryOamAddr = 0;

    for (int i = 0; i < mSpriteCount; ++i) {

        mFgTileY = mSecondaryOam[secondaryOamAddr++];
        mFgTileId = mSecondaryOam[secondaryOamAddr++];
        mFgTileAttribute = mSecondaryOam[secondaryOamAddr++];
        mSpritesXPos[i] = mSecondaryOam[secondaryOamAddr++];

        mFgPatternLo[i] = this->fetchFgTileData(false);
        mFgPatternHi[i] = this->fetchFgTileData(true);
        mFgAttrib[i] = mFgTileAttribute;

    }

}

void PPU2C02::updateShifters(void) {

    if (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND) {
        mBgPatternLo <<= 1; mBgPatternHi <<= 1;
        mBgAttribLo <<= 1; mBgAttribHi <<= 1;
    }

    if (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES
        && mCycle >= 0 && mCycle < 256) {
        for (int i = 0; i < 8; ++i) {
            if (mSpritesXPos[i] > 0) {
                --mSpritesXPos[i];
            } else {
                mFgPatternLo[i] <<= 1; mFgPatternHi[i] <<= 1;
            }
        }
    }
}

void PPU2C02::evaluateOam(void) {

    Byte secondaryOamAddr = 0;

    for (int i = 0; i < 64; ++i) {

        short diff = mScanline - mOam[i * 4]; //difference in Y coord of scanline and sprite
        Byte spriteSize = mRegisters[PPUCTRL] & CTRL_REGISTER::SPRTSIZ ? 16 : 8;
        if (diff < 0 || diff >= spriteSize) { continue; } //sprite not visible

        if (secondaryOamAddr > 31) { //more than 8 sprites on one scanline
            mRegisters[PPUSTATUS] |= STATUS_REGISTER::SPRITE_OVERFLOW;
            break;
        }

        memcpy(&mSecondaryOam[secondaryOamAddr], &mOam[i * 4], 4);
        secondaryOamAddr += 4; //increment by the sprite size
        ++mSpriteCount;

    }

}

void PPU2C02::setSprite0Hit(void) {
    if (mRegisters[PPUSTATUS]
        & STATUS_REGISTER::SPRITE_0)
        return; //ignore if the flag is set

    if (!(mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES)
        || !(mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND))
        return; //both flags need to be set

    if (mCycle == 254)
        return; //this just gets ignored

    mRegisters[PPUSTATUS] |= STATUS_REGISTER::SPRITE_0;
}

void PPU2C02::preRenderRoutine(void) {
    mRegisters[PPUSTATUS] &= ~STATUS_REGISTER::VBLANK;
    mRegisters[PPUSTATUS] &= ~STATUS_REGISTER::SPRITE_0;
    mRegisters[PPUSTATUS] &= ~STATUS_REGISTER::SPRITE_OVERFLOW;

    //clear the sprite data
    memset(mFgPatternLo, 0, 8);
    memset(mFgPatternHi, 0, 8);
    memset(mFgAttrib, 0, 8);
    memset(mSpritesXPos, 0, 8);
}

void PPU2C02::postRenderRoutine(void) {
    mRegisters[PPUSTATUS] |= STATUS_REGISTER::VBLANK;
    if (mRegisters[PPUCTRL] & CTRL_REGISTER::VBNMIEN)
        this->mNmiCallback();
}

void PPU2C02::incrementX(void) {
    if (
        (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_SPRITES)
    ) {
        Byte X = mVRamAddr & VRAM_MASK::COARSE_X;   
        if (X < 31) {
            mVRamAddr = (mVRamAddr & ~VRAM_MASK::COARSE_X) | (++X & COARSE_X);
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
        (mRegisters[PPUMASK] & RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & RENDER_SPRITES)
    ) {
        Byte fineY = (mVRamAddr & FINE_Y) >> 12;
        if (fineY < 7) {
            mVRamAddr = (mVRamAddr & ~FINE_Y) | ((++fineY << 12) & FINE_Y);
            return;
        }
        Byte coarseY = (mVRamAddr & COARSE_Y) >> 5;
        if (coarseY < 29) {
            mVRamAddr = (mVRamAddr & ~(FINE_Y | COARSE_Y)) | ((++coarseY << 5) & COARSE_Y);
            return;
        }
        mVRamAddr = (
            (mVRamAddr & ~(FINE_Y | NT_Y | COARSE_Y) 
            | (~(mVRamAddr & NT_Y) & NT_Y))
        );
    }
}

void PPU2C02::resetX(void) {
    if (
        (mRegisters[PPUMASK] & RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & RENDER_SPRITES)
    ) {
        Word mask = COARSE_X | NT_X;
        mVRamAddr = (
            (mVRamAddr & ~mask)
            | (mTRamAddr & mask)
        );
    }
}

void PPU2C02::resetY(void) {
    if (
        (mRegisters[PPUMASK] & RENDER_BACKGROUND)
        || (mRegisters[PPUMASK] & RENDER_SPRITES)
    ) {
        Word mask = COARSE_Y | FINE_Y | NT_Y;
        mVRamAddr = (
            (mVRamAddr & ~mask)
            | (mTRamAddr & mask)
        );
    }
}
