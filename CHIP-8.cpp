#include "CHIP-8.h"
#include "helperFunctions.h" 

void CHIP8::initialize(){
    sp = 0;
    pc = 0x200;
    current_opcode = 0;
    I = 0;

    for(unsigned long i = 0; i < (sizeof(memory) /sizeof(unsigned char)); i++){
        memory[i] = 0;
    }
}

void CHIP8::load_program(std::string argfile){
    std::ifstream file(argfile);
    std::string fbuffer;
    int i = 0x200;

    std::string HEXrep;
    int v = 1;

    if(!file){
        return;
    }

    while(getline(file, fbuffer)){

        if(fbuffer.length() > 8){
            throw std::invalid_argument( "Binary Value exceeded supported legnth" ); 
        }
        
        HEXrep += fbuffer;

        if(v % 2 == 0){
            std::cout << "OPcode   " << biTOhex(HEXrep) << "\n";
            HEXrep = ""; 
        }

        v++;
        

        std::cout << fbuffer <<"   " << static_cast<int>(biTOdec(fbuffer)) << "\n";
        memory[i++] = biTOdec(fbuffer); 
    }
    

    
}

void CHIP8::FDE(){
    //fetch 
    current_opcode = memory[pc] << 8 | memory[pc + 1];

    //decode + execute
    switch(current_opcode & 0xF000){
        case 0x0000:
            if((current_opcode & 0xFF0F) == 0x0){
                //Clear the screen
                std::cout << "not implemented yet" << std::endl;
                pc += 2;
            }
            else if((current_opcode & 0xFF00) == 0x0){
                //returns from a subroutine
                pc = stack[--sp];
            }
            else{
                //calls machine code subroutine at address NNN
                //(current_opcode & 0xF000) == 0x0
                std::cout << "not necessary" << std::endl;
                pc += 2;
            }
            break;

        case 0x1000:
            //goto Address
            pc = current_opcode & 0x0FFF;
            break;

        case 0x2000:
            //call subroutine
            stack[sp++] = pc + 2;
            pc = current_opcode & 0x0FFF; 
            break;
        
        case 0x3000:
            //skip next instruction if VX == NN
            if(V[(current_opcode & 0x0F00) >> 8] == (current_opcode & 0x00FF)){
                pc += 2;
            }
            pc += 2;
            break;

        case 0x4000:
            //skip next instruction if VX != NN
            if(V[(current_opcode & 0x0F00) >> 8] != (current_opcode & 0x00FF)){
                pc += 2;
            }    
            pc += 2;
            break;
        
        case 0x5000:
            //skip next instruction if VX == VY
            if(V[(current_opcode & 0x0F00) >> 8] == V[(current_opcode & 0x00F0) >> 4]){
                pc += 2;
            }
            pc += 2;
            break;
        
        case 0x6000:
            //Asign VX = NNN
            V[(current_opcode & 0x0F00) >> 8] = (current_opcode & 0x00FF);
            pc += 2;
            break;
        
        case 0x7000:
            //ADDS VX and VY and stores it in VX
            V[(current_opcode & 0x0F00) >> 8] += (current_opcode & 0x00FF);
            pc += 2;
            break;
        case 0x8000:
            switch(current_opcode & 0x000F){
                //Asignes VY to VX
                case 0x0000:
                    V[(current_opcode & 0x0F00) >> 8] = V[(current_opcode & 0x00F0) >> 4];
                    break;
                case 0x0001:
                    //bitwise OR-Operation
                    V[(current_opcode & 0x0F00) >> 8] |= V[(current_opcode & 0x00F0) >> 4];
                    break;
                case 0x0002:
                    //bitwise AND-Operation
                    V[(current_opcode & 0x0F00) >> 8] &= V[(current_opcode & 0x00F0) >> 4];
                    break;
                case 0x0003:
                    //bitwise XOR-Operation
                    V[(current_opcode & 0x0F00) >> 8] ^= V[(current_opcode & 0x00F0) >> 4];
                    break;
                case 0x0004:
                    // ADDS VX and VY and stores it in VX (Carry Flag used) 1 if overflow and 0 if not
                    
                    VF = 0;

                    if(V[(current_opcode & 0xF00) >> 8 ] + V[(current_opcode & 0x00F0) >> 4] > 0x100){
                        VF = 1;
                    }
                    V[(current_opcode & 0x0F00) >> 8] = (unsigned char)(V[(current_opcode & 0x0F00) >> 8] + V[(current_opcode & 0x00F0) >> 4]);

                    break;
                case 0x0005:
                    //Substracts VX from VX and stores it in VX (Carry flag used) 0 if underflow and 1 if not
                    
                    VF = 0;

                    if(V[(current_opcode & 0xF00) >> 8 ] >= V[(current_opcode & 0x00F0) >> 4]){
                        VF = 1;
                    }

                    V[(current_opcode & 0x0F00) >> 8] = (unsigned char)(V[(current_opcode & 0x0F00) >> 8] - V[(current_opcode & 0x00F0) >> 4]);

                    break;
                case 0x0006:
                    //Leftshifts VX to the left by one. The least significant bit (bit which is not inclued anymore) is stored in VF
                    VF = V[(current_opcode & 0x0F00) >> 8] & 0x1;
                    V[(current_opcode & 0x0F00) >> 8] >>= 1;
                    break;
                case 0x0007:
                    //Sets VX to VY - VX (Carry flag used) 0 if underflow and 1 if not
                    
                    VF = 1;
                    if(V[(current_opcode & 0x0F00) >> 8] > V[(current_opcode & 0x00F0) >> 4]) {
                        VF = 0;
                    }

                    V[(current_opcode & 0x0F00) >> 8] = (unsigned char)(V[(current_opcode & 0x00F0) >> 4] - V[(current_opcode & 0x0F00) >> 8]);
                    break;
                case 0x000E:
                    //rightshifts VX to the left by one. The most significant bit (bit which is not inclued anymore) is stored in VF
                    VF = V[(current_opcode & 0x0F00) >> 8] & 0b1000000;
                    V[(current_opcode & 0x0F00) >> 8] <<= 1;
                    break;
            }
            pc += 2;
            break;
            
        case 0x9000:
            //skip next instruction if VX == VY
            if(V[(current_opcode & 0x0F00) >> 8] != V[(current_opcode & 0x00F0) >> 4]){
                pc += 2;
            }
            pc += 2;
            break;
        
        case 0xA000:
            //Sets index Register to address NNN
            I = current_opcode & 0x0FFF;
            pc += 2;
            break;
        
        case 0xB000:
            //Jump to the address NNN + V0
            pc = V[0] + (current_opcode & 0x0FFF);
            break;
        case 0xC000:
            //Assigns random value + NN to VX
            V[(current_opcode & 0x0F00) >> 8] = (std::rand() % 256) + (current_opcode & 0x00FF);
            pc += 2;
            break;
        case 0xD000:
            // Draws Pixel at location VX, VY: More information needed to implement this Opcode (see wikipedia)
            std::cout << "not implemented yet" << std::endl;
            pc += 2;
            break;
        case 0xE000:
            switch(current_opcode & 0x00FF){
                case 0x009E:
                    //Jumps if key is pressed
                    std::cout << "not implemented yet" << std::endl;
                    break;
                    
                case 0x00A1:
                    //jumps if key is not pressed
                    std::cout << "not implemented yet" << std::endl;
                    break;
            }
            pc += 2;

            break;

        case 0xF000:
            switch(current_opcode & 0x00FF){
                case 0x0007:
                    
                    break;
            }
            break;

        default:
            std::cout << "No OPcode found" << std::endl;
    }


}                                                                                   
