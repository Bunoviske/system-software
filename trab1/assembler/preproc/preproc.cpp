#include "preproc.hpp"

//override
FileReader *PreProcessing::getFileReader(string filename)
{
    return new FileReader(filename);
}

//override
FileWriter *PreProcessing::getFileWriter(string filename)
{
    return new FileWriter(filename);
}

//override
void PreProcessing::run(FileReader *rawFile, FileWriter *preprocFile)
{

    int lineNumber = 1;
    bool eof = false;

    while (!eof)
    {

        string line = rawFile->readNextLine();
        if (line == "-1")
            eof = true;
        else
        {
            vector<string> words;
            string preprocLine = "";

            words = getTokensOfLine(line); //retorna apenas palavras diferentes de comentarios, \t e \n. palavras

            //cout << line << endl;
            //cout << "Numero de palavras na linha " << words.size() << endl;

            // if (words.size())
            // { //garante que só chama essa funcao se há palavras
            //     preprocLine = parseWords(lineNumber, words);

            //     if (preprocLine != "")
            //     {
            //         preprocFile->writeNextLine(preprocLine);
            //         lineNumber++;
            //     }
            // }
        }
    }
}

string PreProcessing::parseWords(int lineNumber, vector<string> &words)
{

    //checa se é secao de texto ou dado para saber se MACRO, IF e EQU estão corretas

    //enquanto tiver antes de aparecer alguma secao, checa EQU
    //checa se primeira palavra é rotulo. se for checa se é EQU e se está antes das seções

    //checa se é diretiva IF e se ela está dentro da secao texto.

    //checa se é definicao de MACRO e se está dentro da secao texto

    //checa se é uso de rotulo de uma MACRO e se ela já foi definida

    //analisa primeira palavra, que irá definir como sera essa linha de codigo
    int tokenType = errorService.getLexical(lineNumber).getTokenType(words[0]);

    if (tokenType == INVALID_TOKEN)
    {
        return "";
    }
    else if (tokenType == DEF_LABEL)
    {
        //chama analisador sintatico.
        //Se voltar true, quer dizer que as diretivas contem o numero correto de argumentos

        if (words[1] == "EQU")
        {
        }
        else if (words[1] == "MACRO")
        {
        }
        else
        {
            cout << "DIRETIVA NAO UTILIZADO NO PREPROCESSAMENTO: LINHA" << lineNumber << endl;
        }
    }
    else if (tokenType == LABEL) //nesse caso so pode ser chamando MACRO
    {
        //checkMacroCall();
    }
    else if (tokenType == DIRECTIVE)
    {
        //chamar analisador sintatico
        //Se voltar true, quer dizer que as diretivas contem o numero correto de argumentos

        if (words[0] == "IF")
        {
        }
        else if (words[0] == "SECTION")
        {
            //fazer loop de todas as proximas palavras e chamar getTokenType. So dps chama sintatico passando os tipos
            //de cada token
        }
        else
        {
            cout << "DIRETIVA NAO UTILIZADA NO PREPROCESSAMENTO. LINHA: " << lineNumber << endl;
        }
    }
    else
    {
        cout << "TOKEN TYPE NAO UTILIZADO NO PREPROCESSAMENTO: " << tokenType << endl;
    }
}

vector<string> PreProcessing::getTokensOfLine(string line)
{

    vector<string> words;
    string currentWord = "";

    for (size_t i = 0; i < line.size(); i++)
    {
        char charac = line[i];
        if (charac != ' ' && charac != '\n' && charac != '\t')
        {
            if (charac == ';')
            
                //se for comentário, acaba a linha
                break;

            else{
                currentWord += charac; //se for um caractere valido, adiciona ao token que esta sendo formado
            }
        }
        else if (currentWord != "")
        {
            words.push_back(currentWord);
            currentWord = "";
        }
    }
    if (currentWord != "")
        words.push_back(currentWord);

    return words;
}