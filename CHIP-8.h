#include <fstream>
#include <iostream>
#include <string>
#include "helperFunctions.cpp"

class CHIP8{
    unsigned short pc;
    unsigned short sp;
    unsigned short ip;
    unsigned short current_opcode;

    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short stack[16];

    void initialize();
    void load_opcode();

};