#include <string>

#include "NES/NES.h"
#include "NES/Cartridge/Cartridge.h"

//const std::string filepath = "C:\\Users\\Julek\\Desktop\\Szkoła\\II rok\\projektIndywidualny\\NES_emulator\\tests\\PPU\\ntsc.nes";
const std::string filepath = "C:\\Users\\Julek\\Desktop\\Szkoła\\II rok\\projektIndywidualny\\NES_emulator\\tests\\iceClimber.nes";

int main(void) {
    Cartridge cartridge(filepath);
    NES nes(cartridge);
    nes.run();
}