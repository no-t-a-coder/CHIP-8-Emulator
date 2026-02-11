#include "CHIP-8.h"

void CHIP8::initialize(){
    CHIP8::sp = 0;
    CHIP8::pc = 0x200;
    CHIP8::current_opcode = 0;
    CHIP8::ip = 0;

    for(unsigned long i = 0; i < (sizeof(CHIP8::memory) /sizeof(unsigned char)); i++){
        CHIP8::memory[i] = 0;
    }
}

void CHIP8::load_opcode(){
    std::ifstream file("opcode.bin");
    std::string fbuffer;
    
    if(!file){
        return;
    }

    while(getline(file, fbuffer)){
        std::cout << fbuffer <<"   " << biTOdec(fbuffer) << "\n";
    }
}