#include "macroProcessing.hpp"

int MacroProcessing::getNumberOfMacroArguments(vector<string> &tokens)
{
    return tokens.size() - 2;
    //considerando que a sintaxe já está correta, os dois primeiros tokens sao def_label e diretiva
}
string MacroProcessing::getMacroAssemblyCode(vector<string> &tokens, FileReader *rawFile)
{
    bool macroEnd = false;
    string macroAssemblyCode = "";

    //parametrotokens representa a primeira linha de definicao da macro
    vector<string> arguments = getMacroArguments(tokens); 

    while (!macroEnd)
    {
        string line = rawFile->readNextLine();
        if (line == "-1")
        {
            cout << "Erro semantico: Acabou arquivo sem encontrar END da MACRO. Inserir essa funcao na analise de erro" << endl;
            macroEnd = true;
        }
        else
        {
            vector<string> nextTokens;
            nextTokens = getTokensOfLine(line);            
            if (nextTokens.size() == 1 && boost::to_upper_copy<string>(nextTokens[0]) == "END")
                macroEnd = true;
            else
            {
                //substitui argumentos por #i e monta linha como string para adicionar em macroAssemblyCode
                macroAssemblyCode += assembleMacroLine(nextTokens, arguments);
            }
        }
    }
    //retira o ultimo '\n' da string. Necessario tirar pois a funcao WriteNextLine ja adiciona '\n' no final
    macroAssemblyCode.pop_back();
    return macroAssemblyCode;
}

string MacroProcessing::assembleMacroLine(vector<string> tokens, vector<string> arguments)
{
    string macroLine = "";
    for (int i = 0; i < tokens.size(); i++)
    {
        for (int j = 0; j < arguments.size(); j++)
        {
            string aux = tokens[i];
            if (aux[aux.size() - 1] == ',')
            { //caso argumento esteja como o primeiro argumento de COPY, tira a virgula
                aux.pop_back();
                if (aux == arguments[j])
                    tokens[i] = '#' + to_string(j) + ',';
            }
            else
            {
                if (aux == arguments[j])
                    tokens[i] = '#' + to_string(j);
            }
        }
        //depois de processar o token, adiciona na linha da macro. Se for ultimo token, coloca '\n'
        macroLine += tokens[i] + (i == tokens.size() - 1 ? '\n' : ' ');
    }
    return macroLine;
}

vector<string> MacroProcessing::getMacroArguments(vector<string> &tokens)
{
    vector<string> arguments;
    int numArguments = getNumberOfMacroArguments(tokens);
    for (size_t i = 0; i < numArguments; i++)
    {
        string argument = tokens[i + 2]; //o primeiro argumento está na posicao 2.

        int charOffset = 2; //o padrao é que tenha que tirar & e a virgula
        if (i == numArguments - 1)
            charOffset = 1; //se for o ultimo argumento, nao tem virgula para tirar

        //tira o '&' e a virgula (caso nao seja o ultimo argumento)
        argument = argument.substr(1, argument.size() - charOffset);
        arguments.push_back(argument);
    }
    return arguments;
}