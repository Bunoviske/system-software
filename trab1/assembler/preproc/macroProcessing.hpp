#ifndef MACROPROC
#define MACROPROC

#include "preprocParsingFunction.hpp"
#include <sstream>

class MacroProcessing
{

public:
    MacroProcessing(){};  //construtor
    ~MacroProcessing(){}; //destrutor

    int getNumberOfMacroArguments(vector<string> &tokens, bool isMacroDefinition);
    string getMacroAssemblyCode(vector<string> &tokens, FileReader *rawFile);
    vector<string> expandMacro(vector<string> &tokens); //retorna uma linha de codigo em cada posicao do vetor

private:
    //macro definition
    string assembleMacroLine(vector<string> tokens, vector<string> arguments);

    //macro call
    string substituteArguments(vector<string> tokens, vector<string> arguments);

    //both
    vector<string> getMacroArguments(vector<string> &tokens, bool isMacroDefinition);

};

#endif
