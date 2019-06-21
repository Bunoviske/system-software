#include "baseClass.hpp"


std::string BaseClass::toUpperCase(std::string s){
    std::string temp;
    int i;
    std::locale loc;

    for(i = 0; i < s.length(); i++){
        temp = temp + std::toupper(s[i], loc);
    }

    return temp;
}

vector<string> BaseClass::getTokensOfLine(string line)
{
    vector<string> tokens;
    string currentWord = "";

    for (size_t i = 0; i < line.size(); i++)
    {
        char charac = line[i];
        if (charac != ' ' && charac != '\n' && charac != '\t')
        {
            if (charac == ';')
                //se for comentário, acaba a linha
                break;
            else
                currentWord += charac; //se for um caractere valido, adiciona ao token que esta sendo formado
        }
        else if (currentWord != "")
        {
            tokens.push_back(currentWord);
            currentWord = "";
        }
    }
    if (currentWord != "")
        tokens.push_back(currentWord);

    return tokens;
}

/*************
 * 
 * 
 * LEXICAL ANALYSIS
 * 
 * ************/

int BaseClass::getTokenType(string word)
{

    if (hasInvalidSize(word))
    {
        cout << "Token inválido" << endl;
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

    else if (isValidMacroArgument(word))
        return MACRO_ARGUMENT;

    else if (isLabel(word))
        //precisa ser o ultimo teste do else if pois senao da conflito com isDirective ou isInstruction
        return LABEL;

    else
    {
        cout << "Token inválido" << endl;
        return INVALID_TOKEN;
    }
}

bool BaseClass::isNumerical(string s)
{

    return (isDecimalNumber(s) || isHexadecimalNumber(s));
}


bool BaseClass::isLabelDef(string s)
{
    if (s[s.size() - 1] != ':')
        return false;
    else
    {
        s.pop_back(); //retira o dois pontos
        return isValidLabelToken(s);
    }
}

bool BaseClass::isLabel(string s)
{
    return isValidLabelToken(s);
}

bool BaseClass::isValidLabelToken(string s)
{
    if (!s.empty() && isdigit(s[0]))
        return false;

    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isalnum(*it) || *it == '_'))
        ++it;
    return !s.empty() && it == s.end();
}

bool BaseClass::isInstruction(string s)
{
    return (isValidLabelToken(s) && tables.isInstructionInTable(s));
}
bool BaseClass::isDirective(string s)
{
    return (isValidLabelToken(s) && tables.isDirectiveInTable(s));
}
bool BaseClass::isPlusOperation(string s)
{
    return (s.size() == 1 && s[0] == '+');
}
bool BaseClass::isCopyArgument(string s)
{
    if (s[s.size() - 1] != ',')
        return false;
    else
    {
        s.pop_back(); //retira a virgula
        return isValidLabelToken(s);
    }
}

bool BaseClass::hasInvalidSize(string s)
{
    if (s.size() > 50)
        return true;
    return false;
}

bool BaseClass::isDecimalNumber(string s)
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

bool BaseClass::isHexadecimalNumber(string s)
{
    if (s.size() >= 3 && s[0] == '0' && s[1] == 'X')
    {
        std::string::const_iterator it = s.begin() + 2;
        while (it != s.end() && std::isxdigit(*it))
            ++it;
        return it == s.end();
    }
    return false;
}

bool BaseClass::isValidMacroArgument(string s)
{
    if (!s.empty() && s[0] != '&')
        return false;
    else{ 
        s.erase(s.begin()); //tira &
        if (s[s.size()-1] == ',') //tira a virgula caso exista
            s.pop_back();

        return isValidLabelToken(s);
    }
}