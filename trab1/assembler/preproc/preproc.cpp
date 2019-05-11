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
    //TODO - PROGRAMA DEVE ACEITAR INSTRU/DIRETIVAS/ROTULOS em maiuscula e minuscula.
    //TODO - LABEL PODE SER MESMO NOME QUE INSTRU/DIRETIVA ????

    lineNumber = 1;
    bool eof = false;
    while (!eof)
    {
        string line = rawFile->readNextLine();
        if (line == "-1")
            eof = true;
        else
        {
            vector<string> tokens;

            tokens = getTokensOfLine(line); //retorna apenas palavras diferentes de comentarios, \t e \n. palavras

            //cout << line << endl;
            //cout << "Numero de palavras na linha " << tokens.size() << endl;

            if (tokens.size())
            {                                 //garante que só chama essa funcao se há palavras
                parseTokens(tokens, rawFile); //preprocLine será escrita nessa funcao

                if (preprocLine != "")
                {
                    preprocFile->writeNextLine(preprocLine);
                    lineNumber++;
                }
            }
        }
    }
}

void PreProcessing::assemblePreprocLine(vector<string> &tokens)
{

    for (size_t i = 0; i < tokens.size(); i++)
    {
        preprocLine += tokens[i];

        if (i != tokens.size() - 1)
            preprocLine += ' ';
    }
}

void PreProcessing::parseTokens(vector<string> &tokens, FileReader *rawFile)
{
    preprocLine = ""; //reinicia a string que escreve no arquivo de preproc

    //independente do que for ser feito no preproc, deve-se varrer a linha e verificar sem tem algum equ
    //que deve ser substituido
    changeEquValues(&tokens);

    if (tokensNeedPreproc(tokens))
    //analisa se essa linha requer alguma acao do preprocessamento que requer analise de erro
    {
        //cout << "tokens need preproc" << endl;
        //analisa primeira palavra, que irá definir como sera essa linha de codigo
        int tokenType = errorService.getLexical(lineNumber).getTokenType(tokens[0]);

        if (tokenType == INVALID_TOKEN)
            return;

        else if (tokenType == DEF_LABEL)
            analyseDefLabel(tokens, rawFile);

        else if (tokenType == LABEL) //nesse caso so pode ser chamando MACRO
            analyseMacroCall(tokens);

        else if (tokenType == DIRECTIVE)
            analyseDirective(tokens, rawFile);

        else
        {
            assemblePreprocLine(tokens); //essa parte do codigo é redundante (nunca vai entrar aqui)
            cout << "TOKEN TYPE NAO UTILIZADO NO PREPROCESSAMENTO: " << tokenType << endl;
        }
    }

    else
    {
        //cout << "tokens dont need preproc" << endl;
        assemblePreprocLine(tokens);
    }
}
void PreProcessing::changeEquValues(vector<string> *tokens)
{

    map<string, string> equ = tables.getEquTable();
    // cout << equ.size() << endl;
    // for (map<string, string>::iterator it = equ.begin(); it != equ.end(); ++it)
    // {
    //     cout << it->first << ' ' << it->second << endl;
    // }
    if (equ.size())
    {
        for (size_t i = 0; i < tokens->size(); i++)
        {
            for (map<string, string>::iterator it = equ.begin(); it != equ.end(); ++it)
            {
                if ((*tokens)[i] == it->first)
                    (*tokens)[i] = it->second;
            }
        }
    }
}

bool PreProcessing::tokensNeedPreproc(vector<string> &tokens)
{

    for (size_t i = 0; i < tokens.size(); i++)
    {
        for (size_t j = 0; j < preprocTokens.size(); j++)
        {
            if (tokens[i] == preprocTokens[j])
                return true;
        }
    }
    return false;
}

void PreProcessing::analyseDefLabel(vector<string> &tokens, FileReader *rawFile)
{

    cout << "label definition" << endl;

    //TODO - checar se definicao do label esta na proxima linha

    //Se voltar true, quer dizer que as diretivas contem o numero correto de argumentos, entao
    //é possivel acessá-los sem verificar se vai dar seg fault
    if (errorService.getSintatic(lineNumber).checkLabelDefinitionSintax(tokens))
    {
        if (tokens[1] == "EQU")
        {
            if (errorService.getSemantic(lineNumber).isDirectiveInCorrectSection(tokens[1]) &&
                !errorService.getSemantic(lineNumber).isEquAlreadyDefined(tokens[1]))
            {
                cout << "equ def !!!" << endl;
                tokens[0].pop_back();
                tables.setEquTable(tokens[0], tokens[2]);
            }
        }
        else if (tokens[1] == "MACRO")
        {
            if (errorService.getSemantic(lineNumber).isDirectiveInCorrectSection(tokens[1]) &&
                !errorService.getSemantic(lineNumber).isMacroAlreadyDefined(tokens[0]))
            {
                cout << "Macro def !!!" << endl;
                //TODO
                //tables.setMacroAtTable(tokens[0].pop_back(),getNumberOfMacroArguments(),getMacroAssemblyCode());
                preprocTokens.push_back(tokens[0]); //adiciona no vetor que indica quando deve haver preproc para macro
            }
        }
        else
        {
            assemblePreprocLine(tokens); //redundante
            cout << "DEFINICAO DE LABEL NAO UTILIZADO NO PREPROCESSAMENTO: LINHA" << lineNumber << endl;
        }
    }
}

void PreProcessing::analyseDirective(vector<string> &tokens, FileReader *rawFile)
{
    cout << "diretiva de preproc" << endl;

    //Se voltar true, quer dizer que as diretivas contem o numero correto de argumentos
    if (errorService.getSintatic(lineNumber).checkDirectiveSintax(tokens))
    {
        if (tokens[0] == "IF") // TODO - IF PODE SER TB NA SECAO DE DADOS  ?????????
        {
            if (errorService.getSemantic(lineNumber).isDirectiveInCorrectSection(tokens[0]) &&
                errorService.getSemantic(lineNumber).isLabelInEquTable(tokens[1]))
            {
                if (tokens[1] == "0")
                {
                    // TODO - EQU PODE RECEBER NUMERO NEGATIVO NO IF ?????
                    // se a proxima linha nao puder ser escrita, le a proxima que tem algum conteudo e nao faz nada
                    string line;
                    do
                    {
                        line = rawFile->readNextLine();
                        if (line == "-1")
                        {
                            cout << "Nao ha linha para incluir depois do if" << endl;
                            break;
                        }
                    } while (getTokensOfLine(line).size() == 0);
                }
            }
        }
        else if (tokens[0] == "SECTION")
        {
            errorService.getSemantic(lineNumber).setSectionLine(tokens[1]);
            assemblePreprocLine(tokens);
        }
        else
        {
            assemblePreprocLine(tokens); //redundante
            cout << "DIRETIVA NAO UTILIZADA NO PREPROCESSAMENTO. LINHA: " << lineNumber << endl;
        }
    }
}

void PreProcessing::analyseMacroCall(vector<string> &tokens)
{
    cout << "macro call" << endl;
    //analisa se o numero de parametros esta certo
    if (errorService.getSintatic(lineNumber).checkMacroSintax(tokens))
    {
        // analisa se a macro ja foi definida. Nao analisa se esta na secao certa!
        if (errorService.getSemantic(lineNumber).isMacroInTable(tokens[0]))
        {
            //writeMacroAtPreprocFile(); TODO
            cout << "Macro call !!!" << endl;
        }
    }
}

vector<string> PreProcessing::getTokensOfLine(string line)
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