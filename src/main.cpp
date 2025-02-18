#include <iostream>

#include "NES/NES.h"
#include "Cartridge/Cartridge.h"

std::string filepath = "C:\\Users\\Julek\\Desktop\\Szkoła\\II rok\\projektIndywidualny\\NES_emulator\\nestest.nes";

int main(void) {
    Cartridge cartridge(filepath);
    NES nes(cartridge);
    nes.run();
}