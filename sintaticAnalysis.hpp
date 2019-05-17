#ifndef SINTATIC_ANALYSIS
#define SINTATIC_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"
#include "lexicalAnalysis.hpp"

class SintaticAnalyser : public BaseAnalyser{

public:

    SintaticAnalyser(){
        lexical.setErrorListener(new LexicalListener());
    }; //construtor
    ~SintaticAnalyser(){};

    bool checkInstructionSintax( vector<string>& tokens);
    bool checkDirectiveSintax( vector<string> tokens);
    bool checkLabelDefinitionSintax( vector<string>& tokens);
    bool checkMacroCallSintax( vector<string>& tokens);
    bool checkSectionSintax(vector<string> tokens);

private:

    LexicalAnalyser lexical;
    bool checkSpaceSintax(vector<string> tokens);
    bool checkConstSintax(vector<string> tokens);

};

#endif