#ifndef MACROPROC
#define MACROPROC

#include "../interfaces/preprocInterface.hpp"
#include "../../utils/tablesService.hpp"
#include "../errorAnalysis/errorService.hpp"

#include "../baseClass/baseClass.hpp"
#include <sstream>

class MacroProcessing : private BaseClass
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
    vector<string> getMacroCallArguments(vector<string> &tokens); //pode ter argumentos com offset
    string substituteArguments(vector<string> tokens, vector<string> arguments);

    //both
    vector<string> getMacroArguments(vector<string> &tokens, bool isMacroDefinition);

};

#endif
