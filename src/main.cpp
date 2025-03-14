#pragma warning (disable: 6262) //I'm deliberately allocating most of the app on the stack

#include <iostream>

#include "NES/NES.h"
#include "NES/Cartridge/Cartridge.h"

//FOR TESTING
#include "raylib.h"
#include "IO/Window.h"
#include "NES/APU/APU.h"

void audioDemo() {
    Joypad jp;
    Window* window = Window::getInstance(jp, ScreenOptions{"AudioDemo", 100, 100, 1}, AudioOptions{44100, 16, 1});
    SetTargetFPS(60);
    APU apu(window, 44100);
    while (!WindowShouldClose()) {

        //handle inputs
        if (IsKeyDown(KEY_UP))
            apu.setOscFreq(apu.getOscFreq() + 10);
        if (IsKeyDown(KEY_DOWN))
            apu.setOscFreq(apu.getOscFreq() - 100);
        if (IsKeyDown(KEY_RIGHT))
            apu.setOscShift(apu.getOscShift() + 1);
        if (IsKeyDown(KEY_LEFT))
            apu.setOscShift(apu.getOscShift() - 1);

        window->swapBuffers();
    }
}

int main(int argc, char* argv[]) {

    std::string filepath;

    if (argc != 2) {
        //std::cout << "Incorrect number of arguments. Usage:\n";
        //std::cout << ">./NES_emulator.exe <iNES filepath>\n\n";
        //exit(0);

        audioDemo();
    }

    try {
        Cartridge cartridge(argv[1]);
        NES nes(cartridge);
        nes.run();
    } catch (std::runtime_error& error) {
        std::cout << error.what() << "\n\n";
        exit(0);
    }

}