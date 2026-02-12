#include <string>
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
