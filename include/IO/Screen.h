#ifndef SCREEN_H
#define SCREEN_H

#include <string>

#include "NES/PPU2C02/ColourLUT.h"

class Screen {
public:
    Screen(std::string title, int width, int height);
    ~Screen(void);

    void swapBuffers(void);
    void drawPixel(const int& posX, const int& posY, const Colour& colour);
};

#endif // !SCREEN_H
