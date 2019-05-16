#ifndef BASECLASS
#define BASECLASS

#include <string>
#include <locale>

// using namespace std;


class BaseClass{

public:
    BaseClass() { }; //construtor
    ~BaseClass() { }; //destrutor

    std::string toUpperCase(std::string s);
};




#endif
