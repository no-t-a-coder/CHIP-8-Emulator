#include "CHIP-8.h"
#include "helperFunctions.h" 

void CHIP8::initialize(){
    sp = 0;
    pc = 0x200;
    current_opcode = 0;
    ip = 0;

    for(unsigned long i = 0; i < (sizeof(memory) /sizeof(unsigned char)); i++){
        memory[i] = 0;
    }
}

void CHIP8::load_program(std::string argfile){
    std::ifstream file(argfile);
    std::string fbuffer;
    int i = 0x200;
    if(!file){
        return;
    }

    while(getline(file, fbuffer)){
        std::cout << fbuffer <<"   " << static_cast<int>(biTOdec(fbuffer)) << "\n";
        memory[i++] = biTOdec(fbuffer); 
    }
    

    
}

void CHIP8::FDE(){
    //fetch 
    current_opcode = memory[pc] << 8 | memory[pc + 1];

    //decode + execute
    switch(current_opcode & 0xF000){
        case 0x0:
            if((current_opcode & 0xFF0F) == 0x0){
                //Clear the screen
                std::cout << "not implemented yet" << std::endl;
            }
            else if((current_opcode & 0xFF00) == 0x0){
                //returns from a subroutine
                pc = stack[--sp];
            }
            else{
                //calls machine code subroutine at address NNN
                //(current_opcode & 0xF000) == 0x0
                std::cout << "not necessary" << std::endl;

            }
            break;

        case 0x1:
            //goto Address
            pc = current_opcode & 0x0FFF;
            break;

        case 0x2:
            //call subroutine
            stack[sp++] = pc + 2;
            pc = current_opcode & 0x0FFF; 
            break;
    }


}
