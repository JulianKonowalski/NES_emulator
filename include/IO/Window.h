#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <cstdint>
#include <functional>

#include "raylib.h"

#include "NES/PPU2C02/ColourLUT.h"
#include "IO/Joypad.h"

/**
* Options of the app window.
* The structure stores the
* window title, resolution
* and scale. Scale is specific
* to the NES emulation. 
* Original NES has a resolution 
* of 256x240 pixels, which is
* too little for modern displays.
* Scale multiplies this resolution
* by a given factor.
*/
struct ScreenOptions {
    std::string title = "App";
    unsigned short width = 0;
    unsigned short height = 0;
    uint8_t scale = 1;
};

/**
* Options for the audio device.
* The structure stores the sample rate,
* sample size (bit depth) and number
* of channels.
*/
struct AudioOptions {
    unsigned int sampleRate = 44100;
    unsigned int sampleSize = 16;
    unsigned int channels = 1;
};

/**
* Wrapper class for
* accessing graphics
* and audio APIs.
*/
class Window {
public:

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    /**
    * Returns the current instance
    * of the window class. If the 
    * instance doesn't exist it 
    * creates one and returns it.
    * 
    * @param joypad joypad object that
    *   will store the user input data
    * @param screenOptions screen
    *   configuration options
    * @param audioOptions audio device
    *   configuration options
    * 
    * @return window instance
    * 
    * @see sInstance
    * @see Joypad
    * @see ScreenOptions
    * @see AudioOptions
    */
    static Window* getInstance(Joypad& joypad, const ScreenOptions& screenOptions, const AudioOptions& audioOptions);

    /**
    * Deletes the current instance
    * of the window class. If the
    * instance doesn't exist the 
    * request is ignored.
    * 
    * @see sInstance
    */
    static void destroyInstance(void) { delete sInstance; }

    /**
    * Starts playing the audio stream
    * 
    * @see mAudioStream
    */
    void playAudioStream(void);

    /**
    * Sets the callback of the
    * audio device. This callback
    * will be called upon an empty
    * audio buffer.
    * 
    * @param audioStreamCallback
    *   audio stream callback that
    *   will be called upon an empty
    *   audio buffer.
    * 
    * @see mAudioStreamCallback
    */
    void setAudioStreamCallback(std::function<void(void*, unsigned int)> audioStreamCallback);

    /**
    * Wrapper for the audio stream callback to
    * be passed into RayLib. This is specifically
    * connected with how this library works.
    * 
    * @param buffer audio buffer to be filled
    * @param frames length of the audio buffer
    * 
    * @see mAudioStreamCallback
    */
    static void audioStreamCallback(void* buffer, unsigned int frames);

    /**
    * Returns the current audio buffer size
    * 
    * @return audio buffer size
    * 
    * @see mAudioBufferSize
    */
    short getAudioBufferSize(void) { return mAudioBufferSize; }

    /**
    * Swaps the video buffers, displaying
    * the freshly generated frame.
    */
    void swapBuffers(void);

    /**
    * Draws a pixel to the video buffer.
    * 
    * @param posX x coordinate of the pixel
    * @param posY y coordinate of the pixel
    * @param colour colour of the pixel
    * 
    * @see Colour
    */
    void drawPixel(const int& posX, const int& posY, const Colour& colour);
 
private:

    /**
    * Class constructor. It creates a
    * static instance of the class with 
    * given parameters.
    * 
    * @param joypad joypad object that
    *   will store the user input data
    * @param screenOptions screen
    *   configuration options
    * @param audioOptions audio device
    *   configuration options
    * 
    * @see Joypad
    * @see ScreenOptions
    * @see AudioOptions
    */
    Window(Joypad& joypad, const ScreenOptions& screenOptions, const AudioOptions& audioOptions);

    /**
    * Class destructor. Unloads
    * the audio stream, closes the
    * audio device and closes the
    * application window.
    * 
    * @see mAudioStream
    */
    ~Window(void);

    /**
    * Captures the user input
    * and writes the data
    * to the joypad object.
    * 
    * @see mJoypad
    */
    void handleInputs(void);

    /** Static instance of the window */
    static Window* sInstance;

    /** Callback that will be called upon an empty audio buffer */
    std::function<void(void*, unsigned int)> mAudioStreamCallback;

    /** Joypad that stores the user input data */
    Joypad* mJoypad;

    /** Application's audio stream */
    AudioStream mAudioStream;

    /** Screen scaling factor */
    const short mScale;

    /** Audio buffer size */
    const short mAudioBufferSize;

};

#endif // !SCREEN_H
