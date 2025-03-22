#include "IO/Window.h"

#include <cstdlib>
#include <thread>

Window* Window::sInstance = nullptr;

Window::Window(Joypad& joypad, const ScreenOptions& screenOptions, const AudioOptions& audioOptions) :
    mJoypad(&joypad),
    mScale (screenOptions.scale),
    mAudioBufferSize(4096)
{
    InitWindow(screenOptions.width * mScale, screenOptions.height * mScale, screenOptions.title.c_str());
    SetTargetFPS(60);

    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(mAudioBufferSize);
    mAudioStream = LoadAudioStream(audioOptions.sampleRate, 16, 1);

    BeginDrawing();
    ClearBackground(BLACK);

    std::thread inputHandler(&Window::handleInputs, this);
    inputHandler.detach();
}

Window::~Window(void) { 
    UnloadAudioStream(mAudioStream);
    CloseAudioDevice();
    CloseWindow(); 
}

Window* Window::getInstance(Joypad& joypad, const ScreenOptions& screenOptions, const AudioOptions& audioOptions) {
    if (!sInstance)
        sInstance = new Window(joypad, screenOptions, audioOptions);
    return sInstance;
}

void Window::audioStreamCallback(void* buffer, unsigned int frames) {
    sInstance->mAudioStreamCallback(buffer, frames);
}

void Window::setAudioStreamCallback(std::function<void(void*, unsigned int)> audioStreamCallback) {
    sInstance->mAudioStreamCallback = audioStreamCallback;
    SetAudioStreamCallback(mAudioStream, Window::audioStreamCallback);
}

void Window::playAudioStream(void) {
    PlayAudioStream(mAudioStream);
}

void Window::swapBuffers(void) {
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
}

void Window::drawPixel(const int& posX, const int& posY, const Colour& colour) {
    DrawRectangle(
        posX * mScale, 
        posY * mScale, 
        mScale, 
        mScale, 
        {colour.red(), colour.green(), colour.blue(), 255}
    );
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
