#include "NES/APU/DMC.h"

#include "NES/Buses/CPUBus.h"

DMC::DMC(void) :
  mCpuBus(nullptr),
  mFlags(0),
  mClockCounter(0),
  mBuffer(0),
  mBufferCounter(8),
  mSampleAddress(0),
  mSampleLength(0),
  mOutputLevel(0),
  mSample(0.0f)
{}

void DMC::clock(void) {
  if (!mClockCounter) {
    mClockCounter = sRates[mFlags & DMC_FLAGS::RATE];
    mBuffer <<= 1;
    --mBufferCounter;
    if (!mBufferCounter) {
      mBufferCounter = 8;
      --mSampleLength;
      if (mSampleLength)
        mBuffer = mCpuBus->read(mSampleAddress++);
    }
  }
  --mClockCounter;
  mSample = mBuffer / 0x100;
}

float DMC::process(void) {
  return mSample * mOutputLevel / (float)kMaxOutputLevel;
}

void DMC::writeFlags(const Byte& data) {
  mFlags = data;
}

void DMC::writeDirectLoad(const Byte& data) {
  mOutputLevel = data & 0b01111111;
}

void DMC::writeSampleAddress(const Byte& data) {
  mSampleAddress = 0xC000 | (data << 6);
}

void DMC::writeSampleLength(const Byte& data) {
  mSampleLength = (data << 4) + 0b1;
}
