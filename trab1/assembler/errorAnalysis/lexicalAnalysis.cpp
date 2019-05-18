#include "lexicalAnalysis.hpp"

int LexicalAnalyser::getTokenType(string word)
{

    if (hasInvalidSize(word))
    {
        throwError("Token com tamanho maior que 50 char");
        return INVALID_TOKEN;
    }

    else if (isNumerical(word))
        return NUMBER;

    else if (isLabelDef(word))
        return DEF_LABEL;

    else if (isInstruction(word))
        return INSTRUCTION;

    else if (isDirective(word))
        return DIRECTIVE;

    else if (isPlusOperation(word))
        return PLUS_OPERATION;

    else if (isCopyArgument(word))
        return COPY_ARGUMENT;

    else if (isLabel(word))
        //precisa ser o ultimo teste do else if pois senao da conflito com isDirective ou isInstruction
        return LABEL;

    else
    {
        throwError("Token inválido");
        return INVALID_TOKEN;
    }
}

bool LexicalAnalyser::isLabelDef(string s)
{
    if (s[s.size() - 1] != ':')
        return false;
    else
    {
        s.pop_back(); //retira o dois pontos
        return isValidLabelToken(s);
    }
}
bool LexicalAnalyser::isLabel(string s)
{
    return isValidLabelToken(s);
}

bool LexicalAnalyser::isValidLabelToken(string s)
{
    if (!s.empty() && isdigit(s[0]))
        return false;

    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isalnum(*it) || *it == '_'))
        ++it;
    return !s.empty() && it == s.end();
}

bool LexicalAnalyser::isInstruction(string s)
{
    return (isValidLabelToken(s) && tables.isInstructionInTable(s));
}
bool LexicalAnalyser::isDirective(string s)
{
    return (isValidLabelToken(s) && tables.isDirectiveInTable(s));
}
bool LexicalAnalyser::isPlusOperation(string s)
{
    return (s.size() == 1 && s[0] == '+');
}
bool LexicalAnalyser::isCopyArgument(string s)
{
    if (s[s.size() - 1] != ',')
        return false;
    else
    {
        s.pop_back(); //retira a virgula
        return isValidLabelToken(s);
    }
}

bool LexicalAnalyser::hasInvalidSize(string s)
{
    if (s.size() > 50)
        return true;
    return false;
}

bool LexicalAnalyser::isNumerical(string s){

    return (isDecimalNumber(s) || isHexadecimalNumber(s));
}

bool LexicalAnalyser::isDecimalNumber(string s)
{
    std::string::const_iterator it = s.begin();
    if (s.size() > 0 && s[0] == '-')
        if (s.size() == 1)
            return false; //se for so um traço, retorna false
        else
            it = s.begin() + 1;

    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool LexicalAnalyser::isHexadecimalNumber(string s)
{
    if (s.size() > 3 && s[0] == '0' && s[1] == 'X')
    {
        std::string::const_iterator it = s.begin() + 2;
        while (it != s.end() && std::isxdigit(*it))
            ++it;
        return it == s.end();
    }
    return false;
}
