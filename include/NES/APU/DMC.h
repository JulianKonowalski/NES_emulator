#ifndef DMC_H
#define DMC_H

#include <cstdint>

class CPUBus;

/*
* @brief bitmasks for accessing 
*   the data that's being written 
*   into the flags register
*/
enum DMC_FLAGS {
  IRQE  = 0b10000000, //IRQ enable flag
  LOOP  = 0b01000000, //Loop flag
  RATE  = 0b00001111  //Rate index
};

/*
* @brief rates at which the 
*   sample buffer is shifted 
*   one bit to the left. Rates 
*   are given in APU ticks
*/
static uint8_t sRates[16] = {
  71, 63, 56, 53, 47, 42, 37, 35,
  31, 26, 23, 21, 17, 14, 12, 9
};

/*
* @brief class that 
*   emulates the NES'
*   DMC module behaviour
*/
class DMC {
public:

  using Byte = uint8_t;
  using Word = uint16_t;

  /** max audio output level value */
  static constexpr uint8_t kMaxOutputLevel = 0b01111111;

  /*
  * @brief constructs a 
  *   DMC object
  */
  DMC(void);

  /*
  * @brief clocks the 
  *   DMC module
  */
  void clock(void);

  /*
  * @brief writes data 
  *   to the flags register
  * 
  * @param data data to be 
  *   written into the flags
  *   register
  */
  void writeFlags(const Byte& data);

  /*
  * @brief writes data to 
  *   the 
  */
  void writeDirectLoad(const Byte& data);

  /*
  * @brief writes data to 
  *   the sample address 
  *   register
  * 
  * @param data address of the 
  *   first byte of the sample
  */
  void writeSampleAddress(const Byte& data);

  /*
  * @brief writes data to 
  *   the sample length 
  *   register
  * 
  * @param data length 
  *   of the sample to
  *   be played
  */
  void writeSampleLength(const Byte& data);

  /*
  * @brief returns the value
  *   of the current auio sample
  * 
  * @return current audio sample
  *   value
  */
  float process(void);

  /*
  * @brief updates the address
  *   of the CPU Bus object 
  *   that the DMC module reads 
  *   data from
  * 
  * @param cpuBus pointer to a 
  *   new CPU Bus object
  */
  void setCpuBus(CPUBus* cpuBus) { mCpuBus = cpuBus; }

private:

  /** 
  * pointer to a CPU Bus object
  * that the DMC module reads data 
  * from
  */
  CPUBus* mCpuBus;

  /** Flags register */
  Byte mFlags;

  /** Counter for the clock cycles */
  Byte mClockCounter;

  /** Audio sample buffer */
  Byte mBuffer;

  /** Counter for the buffer shifts */
  Byte mBufferCounter;

  /** Current sample byte address */
  Word mSampleAddress;

  /** Current sample's length in bytes */
  Word mSampleLength;

  /** Audio output level */
  Byte mOutputLevel;

  /** Current sample value */
  float mSample;

};

#endif
