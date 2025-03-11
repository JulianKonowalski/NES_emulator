#include "IO/Window.h"

#include <cstdlib>
#include <thread>

#include "raylib.h"

Window::Window(std::string title, int width, int height, short scale, Joypad& joypad) : mScale (scale), mJoypad(&joypad) {
    InitWindow(width * mScale, height *mScale, title.c_str());
    BeginDrawing();
    ClearBackground(BLACK);
    std::thread inputHandler(&Window::handleInputs, this);
    inputHandler.detach();
}

Window::~Window(void) { CloseWindow(); }

void Window::swapBuffers(void) {
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
}

void Window::drawPixel(const int& posX, const int& posY, const Colour& colour) {
    DrawRectangle(posX * mScale, posY * mScale, mScale, mScale, {colour.red(), colour.green(), colour.blue(), 255});
}

void Window::handleInputs(void) {

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_UP)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_UP, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_UP, false); }

        if (IsKeyDown(KEY_DOWN)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_DOWN, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_DOWN, false); }

        if (IsKeyDown(KEY_LEFT)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_LEFT, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_LEFT, false); }

        if (IsKeyDown(KEY_RIGHT)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_RIGHT, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_RIGHT, false); }

        if (IsKeyDown(KEY_Z)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_SELECT, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_SELECT, false); }

        if (IsKeyDown(KEY_X)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_START, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_START, false); }

        if (IsKeyDown(KEY_A)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_A, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_A, false); }

        if (IsKeyDown(KEY_S)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_B, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_B, false); }
    }
}
