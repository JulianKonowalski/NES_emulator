#include "IO/Window.h"

#include <cstdlib>
#include <fstream>
#include <thread>

#include "json/json.h"

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

#include <iostream>

void Window::handleInputs(void) {
    uint16_t
      p1UP = KEY_W,  p1DN = KEY_S,    p1LT = KEY_A,    p1RT = KEY_D,     p1SL = KEY_Y,    p1ST = KEY_T,    p1BA = KEY_G,    p1BB = KEY_H,
      p2UP = KEY_UP, p2DN = KEY_DOWN, p2LT = KEY_LEFT, p2RT = KEY_RIGHT, p2SL = KEY_KP_6, p2ST = KEY_KP_5, p2BA = KEY_KP_2, p2BB = KEY_KP_3;

    std::ifstream configFile(CONFIG_PATH, std::ifstream::binary);
    if (!configFile.is_open()) { 
        std::cout << "Failed to load keyboard configuration file. Switching to default key bindings.\n";
    } else {
        Json::Value root;
        configFile >> root;

        p1UP = root["KEY_CODES"][root["JOYPAD_1"]["UP"].asString()].asInt();
        p1DN = root["KEY_CODES"][root["JOYPAD_1"]["DOWN"].asString()].asInt();
        p1LT = root["KEY_CODES"][root["JOYPAD_1"]["LEFT"].asString()].asInt();
        p1RT = root["KEY_CODES"][root["JOYPAD_1"]["RIGHT"].asString()].asInt();
        p1SL = root["KEY_CODES"][root["JOYPAD_1"]["SELECT"].asString()].asInt();
        p1ST = root["KEY_CODES"][root["JOYPAD_1"]["START"].asString()].asInt();
        p1BA = root["KEY_CODES"][root["JOYPAD_1"]["A"].asString()].asInt();
        p1BB = root["KEY_CODES"][root["JOYPAD_1"]["B"].asString()].asInt();

        p2UP = root["KEY_CODES"][root["JOYPAD_2"]["UP"].asString()].asInt();
        p2DN = root["KEY_CODES"][root["JOYPAD_2"]["DOWN"].asString()].asInt();
        p2LT = root["KEY_CODES"][root["JOYPAD_2"]["LEFT"].asString()].asInt();
        p2RT = root["KEY_CODES"][root["JOYPAD_2"]["RIGHT"].asString()].asInt();
        p2SL = root["KEY_CODES"][root["JOYPAD_2"]["SELECT"].asString()].asInt();
        p2ST = root["KEY_CODES"][root["JOYPAD_2"]["START"].asString()].asInt();
        p2BA = root["KEY_CODES"][root["JOYPAD_2"]["A"].asString()].asInt();
        p2BB = root["KEY_CODES"][root["JOYPAD_2"]["B"].asString()].asInt();
    }
    
    while (!WindowShouldClose()) {

        if (IsKeyDown(p1UP)) { mJoypad->setButtonState(Joypad::Button::BUTTON_UP, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_UP, false); }

        if (IsKeyDown(p1DN)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_DOWN, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_DOWN, false); }

        if (IsKeyDown(p1LT)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_LEFT, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_LEFT, false); }

        if (IsKeyDown(p1RT)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_RIGHT, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_RIGHT, false); }

        if (IsKeyDown(p1SL)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_SELECT, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_SELECT, false); }

        if (IsKeyDown(p1ST)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_START, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_START, false); }

        if (IsKeyDown(p1BA)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_A, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_A, false); }

        if (IsKeyDown(p1BB)) {  mJoypad->setButtonState(Joypad::Button::BUTTON_B, true); } 
        else { mJoypad->setButtonState(Joypad::Button::BUTTON_B, false); }

    }
}
