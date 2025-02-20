#include "IO/Screen.h"

#include <cstdlib>

#include "raylib.h"

Screen::Screen(std::string title, int width, int height) {
    InitWindow(width, height, title.c_str());
    BeginDrawing();
    ClearBackground(RED);
}

Screen::~Screen(void) { CloseWindow(); }

void Screen::swapBuffers(void) {
    EndDrawing();
    BeginDrawing();
    ClearBackground(RED);
}

void Screen::drawPixel(const int& posX, const int& posY, const Colour& colour) {
    DrawPixel(posX, posY, {colour.red(), colour.green(), colour.blue()});
}
