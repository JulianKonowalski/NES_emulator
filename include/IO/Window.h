#ifndef SCREEN_H
#define SCREEN_H

#include <string>

#include "NES/PPU2C02/ColourLUT.h"
#include "IO/Joypad.h"

class Window {
public:
    Window(std::string title, int width, int height, short scale, Joypad& joypad);
    ~Window(void);

    void swapBuffers(void);
    void drawPixel(const int& posX, const int& posY, const Colour& colour);

private:

    void handleInputs(void);

    short mScale;
    Joypad* mJoypad;
};

#endif // !SCREEN_H
