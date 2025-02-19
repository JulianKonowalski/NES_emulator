#include "IO/Screen.h"

#include "raylib.h"

Screen::Screen(std::string title, int width, int height) {
    InitWindow(width, height, title.c_str());
}

Screen::~Screen(void) { CloseWindow(); }
