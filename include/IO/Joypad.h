#ifndef JOYPAD_H
#define JOYPAD_H

#include <cstdint>

class Joypad {
public:

    using Byte = uint8_t;

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

    Joypad(void) : mState(0), mButtonIndex(0), mStrobe(false) {}

    void setStrobe(const bool& state) { mStrobe = state; }
    void setButtonState(const Joypad::Button& button, const bool& state) {
        if (state) { mState |= button; }
        else { mState &= ~button; }
    }

    Byte read(void) {
        Byte data = mState & (1 << mButtonIndex) ? 1 : 0;
        if (!mStrobe) { mButtonIndex = (mButtonIndex + 1) % 8; }
        return data;
    }


private:

    Byte mState;
    Byte mButtonIndex;
    bool mStrobe;

};

#endif // !JOYPAD_H
