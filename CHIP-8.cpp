#include "CHIP-8.h"
#include "helperFunctions.h" 

CHIP8::~CHIP8(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void CHIP8::initialize(){

    SDL_Init(SDL_INIT_VIDEO);

    PixelScale = 10;

    window = SDL_CreateWindow(
        "CHIP8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        64 * PixelScale,  32 * PixelScale,
        0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    sp = 0;
    pc = 0x200;
    current_opcode = 0;
    I = 0;
    

    run = true;


    for(unsigned long i = 0; i < (sizeof(memory) /sizeof(unsigned char)); i++){
        memory[i] = 0;
    }
    for(unsigned long i = 0; i < sizeof(V) / sizeof(unsigned char); i++){
        V[i] = 0;
    }

    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            gfx[y][x] = 0;
        }
    }
}

void CHIP8::load_program(std::string argfile){
    std::ifstream file(argfile);
    std::string fbuffer;
    int i = 0x200;

    unsigned char chip_8_Fontset[80] = {
        0xF0,0x90,0x90,0x90,0xF0, // 0
        0x20,0x60,0x20,0x20,0x70, // 1
        0xF0,0x10,0xF0,0x80,0xF0, // 2
        0xF0,0x10,0xF0,0x10,0xF0, // 3
        0x90,0x90,0xF0,0x10,0x10, // 4
        0xF0,0x80,0xF0,0x10,0xF0, // 5
        0xF0,0x80,0xF0,0x90,0xF0, // 6
        0xF0,0x10,0x20,0x40,0x40, // 7
        0xF0,0x90,0xF0,0x90,0xF0, // 8
        0xF0,0x90,0xF0,0x10,0xF0, // 9
        0xF0,0x90,0xF0,0x90,0x90, // A
        0xE0,0x90,0xE0,0x90,0xE0, // B
        0xF0,0x80,0x80,0x80,0xF0, // C
        0xE0,0x90,0x90,0x90,0xE0, // D
        0xF0,0x80,0xF0,0x80,0xF0, // E
        0xF0,0x80,0xF0,0x80,0x80  // F
    };

    for(int v = 0; v < 80; v++){
        memory[0x050 + v] = chip_8_Fontset[v];
    }

    std::string HEXrep = "";
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
            std::cout << "OPcode   0x" << biTOhex(HEXrep) << "\n";
            HEXrep = ""; 
        }

        v++;
        

        std::cout << fbuffer <<"   " << static_cast<int>(biTOdec(fbuffer)) << "\n";
        memory[i++] = biTOdec(fbuffer); 
    }

    
}

void CHIP8::emulationCicle(){
    //fetch 
    current_opcode = memory[pc] << 8 | memory[pc + 1];

    //decode + execute
    switch(current_opcode & 0xF000){
        case 0x0000:
            if((current_opcode & 0xFF0F) == 0x0){
                //Clear the screen
                // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                // SDL_RenderClear(renderer);
                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                for(int i = 0; i < 32; i++){
                    for(int v = 0; v < 64; v++){
                        gfx[i][v] = 0;
                    }
                }

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
            //ADDS NN To VX
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
                    
                    V[15] = 0;

                    if(V[(current_opcode & 0xF00) >> 8 ] + V[(current_opcode & 0x00F0) >> 4] > 0x100){
                        V[15] = 1;
                    }
                    V[(current_opcode & 0x0F00) >> 8] = (unsigned char)(V[(current_opcode & 0x0F00) >> 8] + V[(current_opcode & 0x00F0) >> 4]);

                    break;
                case 0x0005:
                    //Substracts VX from VX and stores it in VX (Carry flag used) 0 if underflow and 1 if not
                    
                    V[15] = 0;

                    if(V[(current_opcode & 0xF00) >> 8 ] >= V[(current_opcode & 0x00F0) >> 4]){
                        V[15] = 1;
                    }

                    V[(current_opcode & 0x0F00) >> 8] = (unsigned char)(V[(current_opcode & 0x0F00) >> 8] - V[(current_opcode & 0x00F0) >> 4]);

                    break;
                case 0x0006:
                    //Leftshifts VX to the left by one. The least significant bit (bit which is not inclued anymore) is stored in VF
                    V[15] = V[(current_opcode & 0x0F00) >> 8] & 0x1;
                    V[(current_opcode & 0x0F00) >> 8] >>= 1;
                    break;
                case 0x0007:
                    //Sets VX to VY - VX (Carry flag used) 0 if underflow and 1 if not
                    
                    V[15] = 1;
                    if(V[(current_opcode & 0x0F00) >> 8] > V[(current_opcode & 0x00F0) >> 4]) {
                        V[15] = 0;
                    }

                    V[(current_opcode & 0x0F00) >> 8] = (unsigned char)(V[(current_opcode & 0x00F0) >> 4] - V[(current_opcode & 0x0F00) >> 8]);
                    break;
                case 0x000E:
                    //rightshifts VX to the left by one. The most significant bit (bit which is not inclued anymore) is stored in VF
                    V[15] = V[(current_opcode & 0x0F00) >> 8] & 0b1000000;
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
            V[15] = 0;

            
            for(int row = 0; row < (current_opcode & 0x000F); row++){
                char PixelRow = memory[I + row];

                for(int col = 0; col < 8; col++){
                    if((PixelRow & (0x80 >> col)) != 0){ //if bit is one, draw pixel
                        int xPos = (V[(current_opcode & 0x0F00) >> 8] + col) % 64;
                        int yPos = (V[(current_opcode & 0x00F0) >> 4] + row) % 32;

                        if(gfx[yPos][xPos] == 1){
                            V[15] = 1;
                        }

                        gfx[yPos][xPos] ^= 1;
                    }
                }
            }
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
                    V[(current_opcode & 0x0F00) >> 8] = delay_timer;
                    break;
                case 0x000A:
                    //Key Press is stored in Register VX. Everything is halted (exept delay and sound - timer) until key input was given
                    std::cout << "not implemented yet";
                    break;
                case 0x0015:
                    //delay timer is set to register VX
                    delay_timer = V[(current_opcode & 0x0F00) >> 8];
                    break;
                case 0x0018:
                    //sound timer is set to register VX
                    sound_timer = V[(current_opcode & 0x0F00) >> 8];
                    break;
                case 0x001E:
                    //adds VX to I (Carry flag not used)
                    I += V[(current_opcode & 0x0F00) >> 8];
                    break;
                case 0x0029:
                    //sets I to the address of the lowest pixel of the hex number stored in VX (0-F)
                    I = 0x050 + (V[(current_opcode & 0x0F00) >> 8] * 5);
                    break;
                case 0x0033:
                    //stores BCD representation of the number stored at VX in memory
                    memory[I] = V[(current_opcode & 0x0F00) >> 8] / 100; //hundred digit
                    memory[I + 1] = (V[(current_opcode & 0x0F00) >> 8] / 10) % 100; //ten digit
                    memory[I + 2] = V[(current_opcode & 0x0F00) >> 8] / 10; // one digit
                    break;
                case 0x0055:
                    //stores values from V0 to VX in memory starting at point I
                    for(int i = 0; i < 16; i++){
                        memory[i + I] = V[i];
                    }
                    break;
                case 0x0065:
                    //stores values from memory address I until I + 15 in V0 to VX
                    for(int i = 0; i < 16; i++){
                        V[i] = memory[i + I];
                    }
                    break;
            }
            pc += 2;
            break;

        default:
            std::cout << "No OPcode found" << std::endl;
            throw std::invalid_argument( "Program contains invalid OPcode" ); 
    }

    while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT){
                run = false;
            }
                
        }

    //update timers
    if(delay_timer > 0){
        delay_timer--;
    }
    if(sound_timer > 0){
        if(sound_timer == 1){
            std::cout << "BEEP" << std::endl;
        }
        sound_timer--;
    }


}

void CHIP8::updateScreen(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            if(gfx[y][x] == 1){
                
                for(int offy = 0; offy < PixelScale; offy++){
                    for(int offx = 0; offx < PixelScale; offx++){
                        SDL_RenderDrawPoint(renderer, x * PixelScale + offx, y * PixelScale + offy);
                    }
                }

            }
            
        }
    }

    SDL_RenderPresent(renderer);
}
