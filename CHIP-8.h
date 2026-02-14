#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>


class CHIP8{
    unsigned short pc;
    unsigned short sp;
    unsigned short current_opcode;
    unsigned short I;

    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short stack[16];

    unsigned char VF;


public:
    void initialize();
    void load_program(std::string argfile);
    void FDE();

};
