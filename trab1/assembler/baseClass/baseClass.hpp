#ifndef BASECLASS
#define BASECLASS

#include <iostream>
#include <string>
#include <locale>
#include <vector>

using namespace std;

//basePassage é usado pelo preprocessamento, processamento de macro, passagem 1 e 2
class BaseClass{

public:
    BaseClass() { }; //construtor
    ~BaseClass() { }; //destrutor

protected:

    std::string toUpperCase(std::string s);
    vector<string> getTokensOfLine(string line);
    int checkSpaceSize(vector<string> words);

};




#endif
