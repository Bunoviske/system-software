#include "errorService.hpp"

LexicalAnalyser ErrorService::getLexical(int line)
{
    lexical.setLineNumber(line);
    return lexical;
}

SintaticAnalyser ErrorService::getSintatic(int line)
{
    sintatic.setLineNumber(line);
    return sintatic;
}
SemanticAnalyser ErrorService::getSemantic(int line)
{
    semantic.setLineNumber(line);
    return semantic;
}

void ErrorService::showErrorMessage(string message)
{
}

int ErrorService::getErrorLine(string message) {}
