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

int BaseClass::checkSpaceSize(vector<string> words){
    if(words.size() == 2){   //se a diretiva SPACE tiver argumento, pula o tamanho reservado
        return stoi(words[1]);
    }
    return 1;       //se nao tiver argumento, reserva apenas 1 byte
}
