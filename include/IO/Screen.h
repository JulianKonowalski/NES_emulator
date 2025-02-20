#ifndef SCREEN_H
#define SCREEN_H

#include <string>

#include "NES/PPU2C02/ColourLUT.h"

class Screen {
public:
    Screen(std::string title, int width, int height, short scale);
    ~Screen(void);

    void swapBuffers(void);
    void drawPixel(const int& posX, const int& posY, const Colour& colour);

private:
    short mScale;
};

#endif // !SCREEN_H
