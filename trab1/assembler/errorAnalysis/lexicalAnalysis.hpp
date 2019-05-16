#ifndef LEXICAL_ANALYSIS
#define LEXICAL_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"
#include <boost/algorithm/string.hpp>

#define DEF_LABEL 0
#define LABEL 1
#define INSTRUCTION 2
#define NUMBER 3
#define PLUS_OPERATION 4
#define DIRECTIVE 5
#define INVALID_TOKEN 6
#define COPY_ARGUMENT 7

class LexicalAnalyser : public BaseAnalyser {

public:

    LexicalAnalyser(){}; //construtor
    ~LexicalAnalyser(){};

    int getTokenType(string word);

    bool isNumerical(string s); //acessado pelo analisador semantico

private:

    bool hasInvalidSize(string s);
    bool isDecimalNumber(string s);
    bool isHexadecimalNumber(string s);
    bool isLabelDef(string s);
    bool isLabel(string s);
    bool isInstruction(string s);
    bool isDirective(string s);
    bool isPlusOperation(string s);
    bool isCopyArgument(string s);
    bool isValidLabelToken(string s);

    string toUpperCase(string s);

};

#endif
