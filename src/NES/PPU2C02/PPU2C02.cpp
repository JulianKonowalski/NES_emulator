#include "NES/PPU2C02/PPU2C02.h"

using Byte = PPU2C02::Byte;
using Word = PPU2C02::Word;

PPU2C02::PPU2C02(std::function<void(void)> nmiCallback) :
    mNmiCallback(nmiCallback),
    mWindow(nullptr),
    mBus(nullptr),
    mScanline(-1),
    mCycle(-1),
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
    this->updateState();
    this->draw();
    this->updatePosition();
    if (mScanline == -1 && mCycle == -1) { 
        mWindow->swapBuffers();
    }
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

            /*TODO*/

            break;
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



            break;
        case OAMDATA:



            break;
        case PPUSCROLL:
            if (!mWLatch) {
                mWLatch = 1;
                mXScroll = data & 0b111;
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

void PPU2C02::updateState(void) {

    if (mCycle < 0)
        return;

    if (mScanline < 240) {       //visible + prerender scanline

        if (mScanline == -1) {   //prerender scanline
            if (mCycle == 0)
                this->preRenderRoutine();
            else if (mCycle >= 279 && mCycle < 304)
                this->resetY();
        }

        else if (mCycle < 256 
            || (mCycle >= 320 && mCycle < 336)) 
            this->updateDataRegisters();

        if (mCycle == 255) 
            this->incrementY();
        else if (mCycle == 256)
            this->resetX();

    }
    else if (mScanline == 241 && mCycle == 0) 
        this->postRenderRoutine();
}

void PPU2C02::draw(void) {
    if (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND) {
        Byte shift = 15 - mXScroll;
        Byte pixelCode = ((mPShiftReg1 & (1 << shift)) >> shift) + ((mPShiftReg2 & (1 << shift)) >> shift);
        Byte paletteCode = ((mCShiftReg1 & (1 << shift)) >> shift) + ((mCShiftReg2 & (1 << shift)) >> shift);
        Byte colourCode = mBus->read(0x3F00 + (paletteCode << 2) + pixelCode);
        mWindow->drawPixel(mCycle, mScanline, mColours[colourCode & 0x3F]);
    }
}

void PPU2C02::updatePosition(void) {
    if (++mCycle > 340) {           //341 cycles per scanline
        mCycle = -1;
        if (++mScanline > 261) {    //262 scanlines
            mScanline = -1;
        } 
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

    if (mRegisters[PPUMASK] & MASK_REGISTER::RENDER_BACKGROUND) {
        mPShiftReg1 <<= 1; mPShiftReg2 <<= 1;
        mCShiftReg1 <<= 1; mCShiftReg2 <<= 1;
    }

    switch (mCycle % 8) {
    case RENDER_STAGE::FETCH_NT:
        mPShiftReg1 |= mBgTileLsb; 
        mPShiftReg2 |= mBgTileMsb;
        mCShiftReg1 |= mBgTileAttribute & 0b01 ? 0xFF : 0x00;
        mCShiftReg2 |= mBgTileAttribute & 0b10 ? 0xFF : 0x00;
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
