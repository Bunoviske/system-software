#ifndef SINTATIC_ANALYSIS
#define SINTATIC_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"
#include "lexicalAnalysis.hpp"

class SintaticAnalyser : public BaseAnalyser{

public:

    SintaticAnalyser(){}; //construtor
    ~SintaticAnalyser(){};

    void checkInstructionSintax();
    void checkDirectiveSintax();
    void checkLabelDefinitionSintax();
    void checkLabelAloneSintax();

private:
    
    LexicalAnalyser lexical;

};

#endif