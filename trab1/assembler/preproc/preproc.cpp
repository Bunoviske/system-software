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

    cout << "\nPreprocessing debug\n"
         << endl;

    lineNumber = 1;
    bool eof = false;
    while (!eof)
    {
        string line = toUpperCase(rawFile->readNextLine());
        if (line == "-1")
            eof = true;
        else
            parseCodeLine(line, rawFile, preprocFile);
    }

    cout << "\nEnd Preprocessing debug\n"
         << endl;
}

void PreProcessing::parseCodeLine(string line, FileReader *rawFile, FileWriter *preprocFile)
{
    vector<string> tokens;
    tokens = getTokensOfLine(line); //retorna apenas palavras diferentes de comentarios, \t e \n

    if (tokens.size())
    {                                              //garante que só chama essa funcao se há palavras
        parseTokens(tokens, rawFile, preprocFile); //preprocLine será escrita nessa funcao

        if (preprocLine != "")
        {
            preprocFile->writeNextLine(preprocLine);
            lineNumber++;
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

void PreProcessing::parseTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile)
{
    preprocLine = ""; //reinicia a string que escreve no arquivo de preproc

    //caso seja definicao de label na proxima linha, tokens sera sobrescrito com os tokens corretos da proxima linha
    checkDefLabelInNextLine(&tokens, rawFile);

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
        {
            assemblePreprocLine(tokens); //escreve a linha no arquivo preproc para mostrar que deu erro
            return;
        }
        else if (tokenType == DEF_LABEL)
            analyseDefLabel(tokens, rawFile);

        else if (tokenType == LABEL) //nesse caso so pode ser chamando MACRO
            analyseMacroCall(tokens, rawFile, preprocFile);

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
    if (equ.size())
    {
        for (size_t i = 0; i < tokens->size(); i++)
        {
            for (map<string, string>::iterator it = equ.begin(); it != equ.end(); ++it)
            {
                string aux = (*tokens)[i];
                if (aux[aux.size() - 1] == ',') //caso LABEL esteja como o primeiro argumento de COPY, tira a virgula
                    aux.pop_back();
                //se isso acontecer, é um erro! mas deve fazer a mudança justamente para sinalizar
                //para a 2 passagem q é um argumento invalido. A virgula nao é recolocada, mas nao faz diferenca pois é um erro

                if (aux == it->first)
                    (*tokens)[i] = it->second;
            }
        }
    }
}

void PreProcessing::getLabelDefInNextLine(vector<string> *tokens, FileReader *rawFile)
{
    string nextLine;
    vector<string> nextLineTokens;

    do
    { // enquanto nao achar uma linha com tokens, fica no loop
        nextLine = toUpperCase(rawFile->readNextLine());
        if (nextLine == "-1")
        {
            cout << "Nao ha linha depois do label (vai ser gerado erro sintatico)" << endl;
            break;
        }
        nextLineTokens = getTokensOfLine(nextLine);
    } while (nextLineTokens.size() == 0);

    for (size_t i = 0; i < nextLineTokens.size(); i++)
        tokens->push_back(nextLineTokens[i]); //adiciona a proxima linha depois da definicao do label
}

void PreProcessing::checkDefLabelInNextLine(vector<string> *tokens, FileReader *rawFile)
{
    // - checa se é definicao do label e se a definicao esta na proxima linha.
    // - caso haja erro lexico aqui e nao seja diretiva de preproc, vai ser gerado erro de qualquer jeito.
    //Isso está certo?

    // - a definicao de label sera escrita na mesma linha no arquivo preproc

    if (tokens->size() == 1)
    {
        //nao precisa escrever no arquivo preproc caso dê erro aqui
        if (errorService.getLexical(lineNumber).getTokenType((*tokens)[0]) == DEF_LABEL)
        {
            getLabelDefInNextLine(tokens, rawFile);
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

    //Se voltar true, quer dizer que as diretivas contem o numero correto de argumentos, entao
    //é possivel acessá-los sem verificar se vai dar seg fault
    if (errorService.getSintatic(lineNumber).checkLabelDefinitionSintax(tokens))
    {
        if (tokens[1] == "EQU")
        {
            tokens[0].pop_back(); //tira o dois pontos
            if (errorService.getSemantic(lineNumber).isDirectiveInCorrectSection(tokens[1]) &&
                !errorService.getSemantic(lineNumber).isEquAlreadyDefined(tokens[0]))
            {
                cout << "equ def" << endl;
                tables.setEquTable(tokens[0], tokens[2]);
            }
            else
                assemblePreprocLine(tokens); //mostra no arquivo preproc a linha do erro
        }
        else if (tokens[1] == "MACRO")
        {
            tokens[0].pop_back(); //tira o dois pontos
            if (errorService.getSemantic(lineNumber).isDirectiveInCorrectSection(tokens[1]) &&
                !errorService.getSemantic(lineNumber).isMacroAlreadyDefined(tokens[0]))
            {
                cout << "macro def" << endl;
                tables.setMacroAtTable(tokens[0], macroProcessing.getNumberOfMacroArguments(tokens, true),
                                       macroProcessing.getMacroAssemblyCode(tokens, rawFile));
                preprocTokens.push_back(tokens[0]); //adiciona no vetor que indica quando deve haver preproc para macro
            }
            else
                assemblePreprocLine(tokens); //mostra no arquivo preproc a linha do erro
        }
        else
        {
            assemblePreprocLine(tokens); //redundante
            cout << "DEFINICAO DE LABEL NAO UTILIZADO NO PREPROCESSAMENTO: LINHA" << lineNumber << endl;
        }
    }
    else
        assemblePreprocLine(tokens); //mostra no arquivo preproc a linha do erro
}

void PreProcessing::analyseDirective(vector<string> &tokens, FileReader *rawFile)
{
    cout << "diretiva de preproc" << endl;

    //Se voltar true, quer dizer que as diretivas contem o numero correto de argumentos
    if (errorService.getSintatic(lineNumber).checkDirectiveSintax(tokens))
    {
        if (tokens[0] == "IF")
        { //nao analisa a posicao do IF no codigo pois ela pode ir em qualquer lugar
            if (errorService.getSemantic(lineNumber).isLabelInEquTable(tokens[1]))
            {
                if (tokens[1] == "0")
                {
                    if (isMacroLine)
                        //se for uma linha da macro, nao le do arquivo. apenas sinaliza para pular a proxima linha da macro no loop dela
                        skipNextMacroLine = true;
                    else
                    {
                        cout << "pula proxima" << endl;
                        // TODO - EQU PODE RECEBER NUMERO NEGATIVO NO IF ?????
                        // se a proxima linha nao puder ser escrita, le a proxima que tem algum conteudo e nao faz nada
                        string line;
                        do
                        {
                            line = toUpperCase(rawFile->readNextLine());
                            if (line == "-1")
                            {
                                cout << "Nao ha linha para incluir depois do if" << endl;
                                break;
                            }
                        } while (getTokensOfLine(line).size() == 0);
                    }
                }
            }
            else
                assemblePreprocLine(tokens); //mostra no arquivo preproc a linha do erro
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
    else
        assemblePreprocLine(tokens); //mostra no arquivo preproc a linha do erro
}

void PreProcessing::analyseMacroCall(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile)
{
    cout << "macro call" << endl;
    //analisa se o numero de parametros esta certo
    if (errorService.getSintatic(lineNumber).checkMacroCallSintax(tokens))
    {
        // analisa se a macro ja foi definida e se foi chamada da forma correta. Nao analisa se esta na secao certa!
        if (errorService.getSemantic(lineNumber).isMacroCallCorrect(tokens))
        {

            //expansao da macro
            vector<string> macroLines = macroProcessing.expandMacro(tokens);

            for (size_t i = 0; i < macroLines.size(); i++)
            {
                //rodar o preprocessamento de cada linha da macro
                isMacroLine = true; //variavel que auxilia na logica do IF
                parseCodeLine(macroLines[i], rawFile, preprocFile);

                if (skipNextMacroLine)
                {
                    i++; //pula a proxima linha caso seja IF 0
                    skipNextMacroLine = false;
                }
            }
            //se for expandida uma macro aqui, é necessario resetar preprocLine
            preprocLine = "";
            isMacroLine = false;
        }
        else
            assemblePreprocLine(tokens); //mostra no arquivo preproc a linha do erro
    }
    else
        assemblePreprocLine(tokens); //mostra no arquivo preproc a linha do erro
}
