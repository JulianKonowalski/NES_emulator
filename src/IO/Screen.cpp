#include "IO/Screen.h"

#include <cstdlib>

#include "raylib.h"

Screen::Screen(std::string title, int width, int height, short scale) : mScale (scale) {
    InitWindow(width * mScale, height *mScale, title.c_str());
    BeginDrawing();
    ClearBackground(BLACK);
}

Screen::~Screen(void) { CloseWindow(); }

void Screen::swapBuffers(void) {
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
}

void Screen::drawPixel(const int& posX, const int& posY, const Colour& colour) {
    DrawRectangle(posX * mScale, posY * mScale, mScale, mScale, {colour.red(), colour.green(), colour.blue(), 255});
}
