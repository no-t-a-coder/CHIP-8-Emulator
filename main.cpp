#include <iostream>
#include <fstream>
#include <string>
#include "CHIP-8.h"

int main(){

    CHIP8 chip8;
    chip8.initialize();
    chip8.load_program("opcode.bin");
    chip8.FDE();

    return 0;

}
