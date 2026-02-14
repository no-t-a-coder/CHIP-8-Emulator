
#include "helperFunctions.h"
//#include<iostream>

unsigned char biTOdec(std::string &input){

    unsigned char value = 0;

    for (char c : input) {
        value = value * 2 + (c - '0');
    }
    //std::cout << value << "\n";
    return value;
}

std::string biTOhex(std::string &input){
    
    std::string hexRep;

    if(input.length() % 4 != 0 || input.empty()){
        throw std::invalid_argument("Binary Value not in shape for Hex conversion"); 
    }

    std::map<std::string, std::string> LookUP = {
        {"0000" , "0"},
        {"0001" , "1"},
        {"0010" , "2"},
        {"0011" , "3"},
        {"0100" , "4"},
        {"0101" , "5"},
        {"0110" , "6"},
        {"0111" , "7"},
        {"1000" , "8"},
        {"1001" , "9"},
        {"1010" , "A"},
        {"1011" , "B"},
        {"1100" , "C"},
        {"1101" , "D"},
        {"1110" , "E"},
        {"1111" , "F"}
    };

    for(unsigned long i = 0; i + 3 < input.length(); i += 4){
        for(auto v : LookUP){
            if(v.first == input.substr(i, 4)){
                hexRep += v.second;
            }
        }
    }
    
    return hexRep;

}   
