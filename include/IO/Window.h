#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <cstdint>
#include <functional>

#include "raylib.h"

#include "NES/PPU2C02/ColourLUT.h"
#include "IO/Joypad.h"

struct ScreenOptions {
    std::string title = "App";
    unsigned short width = 0;
    unsigned short height = 0;
    uint8_t scale = 1;
};

struct AudioOptions {
    unsigned int sampleRate = 44100;
    unsigned int sampleSize = 16;
    unsigned int channels = 1;
};

class Window {
public:

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    static Window* getInstance(Joypad& joypad, const ScreenOptions& screenOptions, const AudioOptions& audioOptions);
    static void destroyInstance(void) { delete sInstance; }

    void playAudioStream(void);
    void setAudioStreamCallback(std::function<void(void*, unsigned int)> audioStreamCallback);
    static void audioStreamCallback(void*, unsigned int);
    short getAudioBufferSize(void) { return mAudioBufferSize; }

    void swapBuffers(void);
    void drawPixel(const int& posX, const int& posY, const Colour& colour);
 
private:

    Window(Joypad& joypad, const ScreenOptions& screenOptions, const AudioOptions& audioOptions);
    ~Window(void);

    void handleInputs(void);

    static Window* sInstance;

    std::function<void(void*, unsigned int)> mAudioStreamCallback;

    Joypad* mJoypad;
    AudioStream mAudioStream;

    const short mScale;             //screen scaling
    const short mAudioBufferSize;

};

#endif // !SCREEN_H
