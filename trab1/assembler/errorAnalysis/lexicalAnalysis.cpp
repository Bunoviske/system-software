#include "lexicalAnalysis.hpp"


void LexicalAnalyser::run()
{
    errorListener->onError(currentLine);
}
