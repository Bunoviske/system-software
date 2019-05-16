#include "baseClass.hpp"


std::string BaseClass::toUpperCase(std::string s){
    std::string temp;
    int i;
    std::locale loc;

    for(i = 0; i < s.length(); i++){
        temp = temp + std::toupper(s[i], loc);
    }

    return temp;
}
