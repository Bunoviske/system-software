#ifndef LEXICAL_ANALYSIS
#define LEXICAL_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"

#define DEF_LABEL 0
#define LABEL 1
#define INSTRUCTION 2
#define NUMBER 3
#define PLUS_OPERATION 4
#define DIRECTIVE 5
#define INVALID_TOKEN 6

class LexicalAnalyser : public BaseAnalyser {

public:

    LexicalAnalyser(){}; //construtor
    ~LexicalAnalyser(){};

    int getTokenType(string word);
    
private:
    

};

#endif