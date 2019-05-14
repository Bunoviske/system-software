#ifndef SINTATIC_ANALYSIS
#define SINTATIC_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"
#include "lexicalAnalysis.hpp"

class SintaticAnalyser : public BaseAnalyser{

public:

    SintaticAnalyser(){}; //construtor
    ~SintaticAnalyser(){};

    bool checkInstructionSintax( vector<string>& tokens);
    bool checkDirectiveSintax( vector<string>& tokens);
    bool checkLabelDefinitionSintax( vector<string>& tokens);
    bool checkMacroCallSintax( vector<string>& tokens); 

private:
    
    LexicalAnalyser lexical;

};

#endif