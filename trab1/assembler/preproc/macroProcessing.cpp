#include "macroProcessing.hpp"

/************* MACRO DEFINITION ************/

int MacroProcessing::getNumberOfMacroArguments(vector<string> &tokens, bool isMacroDefinition)
{
    //offset determina a partir de qual token começa a analisa dos argumentos
    //em macro definition, offset = 2. em macro call, offset = 1.
    int offset = isMacroDefinition ? 2 : 1;

    return tokens.size() - offset;
    //considerando que a sintaxe já está correta, os dois primeiros tokens sao def_label e diretiva para macro def.
    //para macro call, o primeiro token é o label
}
string MacroProcessing::getMacroAssemblyCode(vector<string> &tokens, FileReader *rawFile)
{
    bool macroEnd = false;
    string macroAssemblyCode = "";

    //parametro tokens representa a primeira linha de definicao da macro
    vector<string> arguments = getMacroArguments(tokens, true);

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
            if (nextTokens.size() == 1 && toUpperCase(nextTokens[0]) == "END")
                macroEnd = true;
            else
            {
                //substitui argumentos por #i e monta linha como string para adicionar em macroAssemblyCode
                macroAssemblyCode += assembleMacroLine(nextTokens, arguments);
            }
        }
    }
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

vector<string> MacroProcessing::getMacroArguments(vector<string> &tokens, bool isMacroDefinition)
{
    //offset determina a partir de qual token começa a analisa dos argumentos
    //em macro definition, offset = 2. em macro call, offset = 1.
    int offset = isMacroDefinition ? 2 : 1;

    vector<string> arguments;
    int numArguments = getNumberOfMacroArguments(tokens, isMacroDefinition);
    for (size_t i = 0; i < numArguments; i++)
    {
        string argument = tokens[i + offset]; //o primeiro argumento está na posicao 2.

        int charOffset = offset; //o padrao é que tenha que tirar & e a virgula em macro def e apenas a virgula em macro call
        if (i == numArguments - 1)
            charOffset--; //se for o ultimo argumento, nao tem virgula para tirar

        //tira o '&' e a virgula (caso nao seja o ultimo argumento) em macro def.
        //tira a virgula (caso nao seja o ultimo argumento) em macro call.
        argument = argument.substr(isMacroDefinition ? 1 : 0, argument.size() - charOffset);
        arguments.push_back(argument);
    }
    return arguments;
}

/************* MACRO CALL ************/

vector<string> MacroProcessing::expandMacro(vector<string> &tokens)
{

    // a sintaxe da macro esta correta, entao pode acessar elementos diretamente
    string macroAssemblyCode = tables.getMacroAssemblyCode(tokens[0]);
    istringstream aux(macroAssemblyCode);

    vector<string> arguments = getMacroArguments(tokens, false); //pega os parametros

    vector<string> macroLines;      //variavel de retorno contendo as linhas da macro ja substituidas pelos parametros
    vector<string> macroLineTokens; //tokens de cada linha da macro. Utilizado para substituir #i por parametros
    string line;                    //cada linha da macro

    while (getline(aux, line))
    {
        macroLineTokens = getTokensOfLine(line);
        line = substituteArguments(macroLineTokens, arguments);
        macroLines.push_back(line);
    }
    return macroLines;
}

string MacroProcessing::substituteArguments(vector<string> tokens, vector<string> arguments)
{
    string macroLine = "";

    for (int i = 0; i < tokens.size(); i++)
    {
        string aux = tokens[i];
        if (aux[0] == '#') //garante-se que existe um proximo caracter pois foi o proprio preproc q escreveu na tabela
        {
            int argNumber = atoi(&aux[1]);
            if (aux[aux.size() - 1] == ',')
                tokens[i] = arguments[argNumber] + ',';
            else
                tokens[i] = arguments[argNumber];
        }

        macroLine += tokens[i] + ' ';
    }

    macroLine.pop_back(); //tira o ultimo espaco do ultimo token!!!
    return macroLine;
}
