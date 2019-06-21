#ifndef BASECLASS
#define BASECLASS

#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include "../../utils/tablesService.hpp"

#define DEF_LABEL 0
#define LABEL 1
#define INSTRUCTION 2
#define NUMBER 3
#define PLUS_OPERATION 4
#define DIRECTIVE 5
#define INVALID_TOKEN 6
#define COPY_ARGUMENT 7
#define MACRO_ARGUMENT 8

using namespace std;

//basePassage é usado pelo preprocessamento
class BaseClass
{

public:
    BaseClass(){};  //construtor
    ~BaseClass(){}; //destrutor

protected:
    int getTokenType(string word);
    std::string toUpperCase(std::string s);
    vector<string> getTokensOfLine(string line);
    bool isDecimalNumber(string s);
    bool isHexadecimalNumber(string s);

private:
    bool isNumerical(string s);
    bool hasInvalidSize(string s);
    bool isLabelDef(string s);
    bool isLabel(string s);
    bool isInstruction(string s);
    bool isDirective(string s);
    bool isPlusOperation(string s);
    bool isCopyArgument(string s);
    bool isValidLabelToken(string s);
    bool isValidMacroArgument(string s);
};

#endif
