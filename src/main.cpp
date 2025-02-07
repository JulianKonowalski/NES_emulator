#include <iostream>

#include <NES/NES.h>

const std::string filepath = "C:/Users/Julek/Desktop/Szkoła/II rok/projektIndywidualny/NES_emulator/test.bin";

int main(void) {
    NES nes(filepath);

    //delete startup on 0x400 on the CPU
    nes.run();
}