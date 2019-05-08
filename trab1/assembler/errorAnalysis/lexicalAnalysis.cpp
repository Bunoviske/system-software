#include "lexicalAnalysis.hpp"


void LexicalAnalyser::run()
{
    throwError();
}

int LexicalAnalyser::getTokenType(string word){

    tables.isDirectiveInTable(word);
    return DIRECTIVE;


    throwError();
    return INVALID_TOKEN;

}
