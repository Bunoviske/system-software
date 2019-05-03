#include "sintaticAnalysis.hpp"


void SintaticAnalyser::run()
{
    errorListener->onError(currentLine);
}