#pragma warning (disable: 6262) //I'm deliberately allocating most of the app on the stack

#include <iostream>

#include "NES/NES.h"
#include "NES/Cartridge/Cartridge.h"

int main(int argc, char* argv[]) {

    std::string filepath;

    if (argc != 2) {
        //std::cout << "Incorrect number of arguments. Usage:\n";
        //std::cout << ">./NES_emulator.exe <iNES filepath>\n\n";
        //exit(0);

        filepath = "C:\\Users\\Julek\\Desktop\\Szkoła\\II rok\\projektIndywidualny\\NES_emulator\\tests\\superMarioBros.nes";
    } else {
        filepath = argv[1];
    }

    try {
        Cartridge cartridge(filepath);
        NES nes(cartridge);
        nes.run();
    } catch (std::runtime_error& error) {
        std::cout << error.what() << "\n\n";
        exit(0);
    }

}