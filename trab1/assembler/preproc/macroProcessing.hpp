#ifndef MACROPROC
#define MACROPROC

#include "preprocParsingFunction.hpp"

class MacroProcessing {

public:

    MacroProcessing(){}; //construtor
    ~MacroProcessing(){}; //destrutor
    
    int getNumberOfMacroArguments(vector<string> &tokens);
    string getMacroAssemblyCode(vector<string> &tokens,FileReader *rawFile);

private:

    vector<string> getMacroArguments(vector<string> &tokens);
    string assembleMacroLine(vector<string> tokens, vector<string> arguments);
};

#endif
    
    
    
