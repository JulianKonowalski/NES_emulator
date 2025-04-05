#ifndef JOYPAD_H
#define JOYPAD_H

#include <cstdint>

/**
* Emulates the behaviour 
* of the NES' joypad. It 
* holds the state of the 
* input and passes it bit 
* by bit to the calling 
* class using an 8-bit 
* shift register.
*/
class Joypad {
public:

    using Byte = uint8_t;

    /**
    * Available buttons. The
    * values indicate the position
    * of the button in the joypad's
    * state
    * 
    * @see mState
    */
    enum Button {
        BUTTON_A = 1,
        BUTTON_B = 1 << 1,
        BUTTON_SELECT = 1 << 2,
        BUTTON_START = 1 << 3,
        BUTTON_UP = 1 << 4,
        BUTTON_DOWN = 1 << 5,
        BUTTON_LEFT = 1 << 6,
        BUTTON_RIGHT = 1 << 7
    };
    
    /**
    * Class constructor. Initializes the
    * class member variables with default
    * values.
    */
    Joypad(void) : mState(0), mButtonIndex(0), mStrobe(false) {}

    /**
    * Sets the value of the
    * strobe.
    * 
    * @param state the state
    *   of the strobe.
    * 
    * @see mStrobe
    */
    void setStrobe(const bool& state) { mStrobe = state; }

    /**
    * Sets the state of one
    * of the joypad's buttons.
    * The state is stored in the
    * internal state register.
    * 
    * @param button the
    *   button to be set or cleared.
    * @param state the state
    *   of the button.
    * 
    * @see Button
    * @see mState
    */
    void setButtonState(const Joypad::Button& button, const bool& state) {
        if (state) { mState |= button; }
        else { mState &= ~button; }
    }

    /**
    * Returns the next value
    * in the internal state
    * register. Increments
    * the button index after the
    * operation. If the strobe
    * is set, the button index
    * is not incremented.
    * 
    * @return the state of the
    *   button pointed to by
    *   the button index.
    * 
    * @see mState
    * @see mButtonIndex
    * @see mStrobe
    */
    Byte read(void) {
        Byte data = mState & (1 << mButtonIndex) ? 1 : 0;
        if (!mStrobe) { mButtonIndex = (mButtonIndex + 1) % 8; }
        return data;
    }


private:
    
    /** Current state of the joypad's buttons */
    Byte mState;

    /** Index used to cycle over the state register */
    Byte mButtonIndex;

    /** State of the strobe */
    bool mStrobe;

};

#endif // !JOYPAD_H
