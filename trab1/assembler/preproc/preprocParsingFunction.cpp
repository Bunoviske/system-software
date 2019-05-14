#include "preprocParsingFunction.hpp"

vector<string> getTokensOfLine(string line)
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