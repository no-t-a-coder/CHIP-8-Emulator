#pragma once

#include <fstream>
#include <iostream>
#include <string>


class CHIP8{
    unsigned short pc;
    unsigned short sp;
    unsigned short ip;
    unsigned short current_opcode;

    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short stack[16];


public:
    void initialize();
    void load_program(std::string argfile);
    void FDE();

};
