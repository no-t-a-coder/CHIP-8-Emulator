#pragma once

#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>


class CHIP8{

    

    unsigned short pc;
    unsigned short sp;
    unsigned short current_opcode;
    unsigned short I;

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short stack[16];
    unsigned char gfx[32][64];

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    

public:

    bool run;
    unsigned char PixelScale;

    ~CHIP8();
    void initialize();
    void load_program(std::string argfile);
    void emulationCicle();
    void updateScreen();

};
