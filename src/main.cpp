#include <iostream>

#include <NES/NES.h>

const std::string filepath = "C:/Users/Julek/Desktop/Szkoła/II rok/projektIndywidualny/NES_emulator/test.bin";

int main(void) {
    NES nes(filepath);
    nes.run();
}