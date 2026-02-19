
#include <iostream>
#include <fstream>
#include <string>
#include "CHIP-8.h"
#include "helperFunctions.h"
int main(){


    CHIP8 chip8;
    chip8.initialize();
    chip8.load_program("opcode.bin");


    while(chip8.run){
        //emulation cicle
        chip8.emulationCicle();
        chip8.updateScreen();

        
    }

    return 0;

}
