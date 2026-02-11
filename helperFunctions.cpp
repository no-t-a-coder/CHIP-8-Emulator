#include<string>
#include<iostream>
#include<cmath> 
int biTOdec(std::string &input){

    int value = 0;

    for (char c : input) {
    value = value * 2 + (c - '0');
}
    std::cout << value << "\n";
    return value;
}