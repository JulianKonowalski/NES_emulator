#include <iostream>

#include "NES/NES.h"
#include "NES/Cartridge/Cartridge.h"

const std::string filepath = "C:\\Users\\Julek\\Desktop\\Szkoła\\II rok\\projektIndywidualny\\NES_emulator\\nestest.nes";

int main(void) {
    Screen screen("NES", 256, 240, 4);
    Cartridge cartridge(filepath);
    NES nes(screen, cartridge);
    nes.run();
}