#include "semanticAnalysis.hpp"

void SemanticAnalyser::run()
{
    errorListener->onError(currentLine);
}
