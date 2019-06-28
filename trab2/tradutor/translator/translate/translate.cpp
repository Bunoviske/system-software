#include "translate.hpp"

//override
FileReader *TranslateProcessing::getFileReader(string filename)
{
    return new FileReader(filename);
}

//override
FileWriter *TranslateProcessing::getFileWriter(string filename)
{
    return new FileWriter(filename);
}

//override
void TranslateProcessing::run(FileReader *rawFile, FileWriter *preprocFile)
{

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
}

void TranslateProcessing::importAssemblyFunctions(FileWriter* preprocFile){
    FileReader* functionsFile = getFileReader("./translator/translate/functions.asm");
    bool eof = false;

    while(!eof){
        string line = toUpperCase(functionsFile->readNextLine());
        if (line == "-1"){
            eof = true;
        }
        else{
            //write line to file
            preprocFile->writeNextLine(line);
        }
    }
}

void TranslateProcessing::parseCodeLine(string line, FileReader *rawFile, FileWriter *preprocFile)
{
    importAssembly = false;
    vector<string> tokens;
    tokens = getTokensOfLine(line); //retorna apenas palavras diferentes de comentarios, \t e \n

    if (tokens.size())
    { //garante que só chama essa funcao se há palavras
        if (!preprocTokens(tokens, rawFile, preprocFile))
        {
            translateTokens(tokens, rawFile, preprocFile);

            if(importAssembly){
                importAssemblyFunctions(preprocFile);
                importAssembly = false;
            }
            if (translatedLine != "")
            {
                preprocFile->writeNextLine(translatedLine);
                lineNumber++;
            }
        }
    }
}

void TranslateProcessing::translateTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile)
{
    translatedLine = ""; //reinicia a string que escreve no arquivo de preproc

    int tokenType = getTokenType(tokens[0]);
    if (tokenType == DEF_LABEL)
        analyseDefLabel(tokens, rawFile);

    else if (tokenType == INSTRUCTION)          //nesse caso so pode ser chamando MACRO
        analyseInstruction(tokens, rawFile, 1); //instrucao sem definicao de label antes

    else if (tokenType == DIRECTIVE)
        analyseDirective(tokens, rawFile);

    //assembletranslatedLine(tokens);
}

string TranslateProcessing::getLabelWithDisplacement(vector<string> &tokens, int begin)
{
    //label pode ter offset (displacement). Se nao tiver, essa funcao retorna o label normalmente

    //TODO - COPY e S_OUTPUT/INPUT nao aceita displacement

    string structure = "";
    for (size_t i = begin; i < tokens.size(); i++)
    { //caso seja um numero, tem que multiplicar por 4 (enderecamento de 32 bits = 4 bytes)
        if (isDecimalNumber(tokens[i]))
        {
            int offset = 4 * stoi(tokens[i]);
            stringstream ss;
            ss << offset;
            structure += ss.str();
        }
        else
        {
            structure += tokens[i];
        }
    }
    return structure;
}

void TranslateProcessing::analyseInstruction(vector<string> &tokens, FileReader *rawFile, int variableBegin)
{
    if (tokens[variableBegin - 1] == "ADD")
    {
        translatedLine += "ADD EAX,[" + getLabelWithDisplacement(tokens, variableBegin) + ']';
    }
    else if (tokens[variableBegin - 1] == "SUB")
    {
        translatedLine += "SUB EAX,[" + getLabelWithDisplacement(tokens, variableBegin) + ']';
    }
    else if (tokens[variableBegin - 1] == "MULT")
    {
        translatedLine += "IMUL DWORD [" + getLabelWithDisplacement(tokens, variableBegin) + ']';
    }
    else if (tokens[variableBegin - 1] == "DIV")
    {
        translatedLine += "MOV EDX,0\nIDIV DWORD [" + getLabelWithDisplacement(tokens, variableBegin) + ']';
    }
    else if (tokens[variableBegin - 1] == "JMP")
    {
        translatedLine += "JMP " + tokens[variableBegin];
    }
    else if (tokens[variableBegin - 1] == "JMPN")
    {
        translatedLine += "CMP EAX,0\nJL " + tokens[variableBegin];
    }
    else if (tokens[variableBegin - 1] == "JMPP")
    {
        translatedLine += "CMP EAX,0\nJG " + tokens[variableBegin];
    }
    else if (tokens[variableBegin - 1] == "JMPZ")
    {
        translatedLine += "CMP EAX,0\nJE " + tokens[variableBegin];
    }
    else if (tokens[variableBegin - 1] == "COPY")
    {
        tokens[1].pop_back(); //tira a virgula
        translatedLine += "PUSH EAX\nMOV EAX,[" + tokens[variableBegin] +
                         "]\nMOV [" + tokens[variableBegin + 1] + "],EAX\nPOP EAX";
    }
    else if (tokens[variableBegin - 1] == "LOAD")
    {
        translatedLine += "MOV EAX,[" + getLabelWithDisplacement(tokens, variableBegin) + ']';
    }
    else if (tokens[variableBegin - 1] == "STORE")
    {
        translatedLine += "MOV [" + getLabelWithDisplacement(tokens, variableBegin) + "],EAX";
    }
    else if (tokens[variableBegin - 1] == "STOP")
    {
        translatedLine += "MOV EAX,1\nMOV EBX,0\nINT 0X80";
    }
    else if (tokens[variableBegin - 1] == "INPUT")
    {
        translatedLine += "PUSH " + getLabelWithDisplacement(tokens, variableBegin);
        translatedLine += "\nCALL " + INPUT_FUNCTION;
    }
    else if (tokens[variableBegin - 1] == "OUTPUT")
    {
        translatedLine += "PUSH DWORD [" + getLabelWithDisplacement(tokens, variableBegin) + "]";
        translatedLine += "\nCALL " + OUTPUT_FUNCTION;
    }
    else if (tokens[variableBegin - 1] == "C_INPUT")
    {
        translatedLine += "PUSH " + getLabelWithDisplacement(tokens, variableBegin);
        translatedLine += "\nCALL " + C_INPUT_FUNCTION;
    }
    else if (tokens[variableBegin - 1] == "C_OUTPUT")
    {
        translatedLine += "PUSH DWORD [" + getLabelWithDisplacement(tokens, variableBegin) + "]";
        translatedLine += "\nCALL " + C_OUTPUT_FUNCTION;
    }
    else if (tokens[variableBegin - 1] == "H_INPUT")
    {
        translatedLine += "PUSH " + getLabelWithDisplacement(tokens, variableBegin);
        translatedLine += "\nCALL " + H_INPUT_FUNCTION;
    }
    else if (tokens[variableBegin - 1] == "H_OUTPUT")
    {
        translatedLine += "PUSH DWORD [" + getLabelWithDisplacement(tokens, variableBegin) + "]";
        translatedLine += "\nCALL " + H_OUTPUT_FUNCTION;
    }
    else if (tokens[variableBegin - 1] == "S_INPUT")
    {
        tokens[1].pop_back(); //tira a virgula
        translatedLine += "PUSH " + tokens[variableBegin] + "\nPUSH " + tokens[variableBegin + 1];
        translatedLine += "\nCALL " + S_INPUT_FUNCTION;
    }
    else if (tokens[variableBegin - 1] == "S_OUTPUT")
    {
        tokens[1].pop_back(); //tira a virgula
        translatedLine += "PUSH " + tokens[variableBegin] + "\nPUSH " + tokens[variableBegin + 1];
        translatedLine += "\nCALL " + S_OUTPUT_FUNCTION;
    }
}

void TranslateProcessing::analyseDefLabel(vector<string> &tokens, FileReader *rawFile)
{
    //cout << "label definition" << endl;
    if (tokens[1] == "EQU")
    {
        tokens[0].pop_back(); //tira o dois pontos
        //cout << "equ def" << endl;
        tables.setEquTable(tokens[0], tokens[2]);
    }
    else if (tokens[1] == "SPACE")
    {
        int spaceSize = 1;
        string arrayDeclaration = "";
        if (tokens.size() > 2)
        {
            if (isHexadecimalNumber(tokens[2]))
            {
                spaceSize = stoi(tokens[2], nullptr, 16);
            }
            else if (isDecimalNumber(tokens[2]))
            {
                spaceSize = stoi(tokens[2]);
            }
        }
        for (size_t i = 0; i < spaceSize; i++)
        {
            arrayDeclaration += "0,";
        }
        arrayDeclaration.pop_back();
        translatedLine = tokens[0] + " dd " + arrayDeclaration;
    }
    else if (tokens[1] == "CONST")
    {
        translatedLine = tokens[0] + " dd " + tokens[2];
    }
    else if (getTokenType(tokens[1]) == INSTRUCTION)
    {
        translatedLine = tokens[0] + " ";
        analyseInstruction(tokens, rawFile, 2); //instrucao COM definicao de label antes
    }
    else
    {
        cout << "Definicao de label invalida" << endl;
    }
}

void TranslateProcessing::analyseDirective(vector<string> &tokens, FileReader *rawFile)
{
    //cout << "diretiva de preproc" << endl;

    if (tokens[0] == "IF")
    { //nao analisa a posicao do IF no codigo pois ela pode ir em qualquer lugar
        if (tokens[1] == "0")
        {
            //cout << "pula proxima" << endl;
            // se a proxima linha nao puder ser escrita, le a proxima que tem algum conteudo e nao faz nada
            string line;
            do
            {
                line = toUpperCase(rawFile->readNextLine());
                if (line == "-1")
                {
                    //cout << "Nao ha linha para incluir depois do if" << endl;
                    break;
                }
            } while (getTokensOfLine(line).size() == 0);
        }
    }
    else if (tokens[0] == "SECTION")
    {
        if (tokens[1] == "TEXT")
            translatedLine = "SECTION .text\nGLOBAL _start\n_start:";
        else if (tokens[1] == "DATA"){
            importAssembly = true;
            translatedLine = "SECTION .data";
        }
        else
            cout << "Diretiva invalida" << endl;
    }
    else
    {
        cout << "Diretiva invalida" << endl;
    }
}

void TranslateProcessing::assembletranslatedLine(vector<string> &tokens)
{

    for (size_t i = 0; i < tokens.size(); i++)
    {
        translatedLine += tokens[i];

        if (i != tokens.size() - 1)
            translatedLine += ' ';
    }
}

bool TranslateProcessing::preprocTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile)
{
    //caso seja definicao de label na proxima linha, tokens sera sobrescrito com os tokens corretos da proxima linha
    checkDefLabelInNextLine(&tokens, rawFile);

    //independente do que for ser feito no preproc, deve-se varrer a linha e verificar sem tem algum equ
    //que deve ser substituido
    changeEquValues(&tokens);

    //cout << "tokens need preproc" << endl;
    //analisa primeira palavra, que irá definir como sera essa linha de codigo
    int tokenType = getTokenType(tokens[0]);

    if (tokenType == INVALID_TOKEN)
    {
        cout << "Token invalido" << endl; //escreve a linha no arquivo preproc para mostrar que deu erro
        return false;
    }
    else if (tokenType == DEF_LABEL && tokens[1] == "EQU")
    {
        analyseDefLabel(tokens, rawFile);
        return true;
    }

    else if (tokenType == DIRECTIVE && tokens[0] == "IF")
    {
        analyseDirective(tokens, rawFile);
        return true;
    }
    return false;
}
void TranslateProcessing::changeEquValues(vector<string> *tokens)
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

void TranslateProcessing::getLabelDefInNextLine(vector<string> *tokens, FileReader *rawFile)
{
    string nextLine;
    vector<string> nextLineTokens;

    do
    { // enquanto nao achar uma linha com tokens, fica no loop
        nextLine = toUpperCase(rawFile->readNextLine());
        if (nextLine == "-1")
        {
            //cout << "Nao ha linha depois do label (vai ser gerado erro sintatico)" << endl;
            break;
        }
        nextLineTokens = getTokensOfLine(nextLine);
    } while (nextLineTokens.size() == 0);

    for (size_t i = 0; i < nextLineTokens.size(); i++)
        tokens->push_back(nextLineTokens[i]); //adiciona a proxima linha depois da definicao do label
}

void TranslateProcessing::checkDefLabelInNextLine(vector<string> *tokens, FileReader *rawFile)
{
    // - checa se é definicao do label e se a definicao esta na proxima linha.
    // - caso haja erro lexico aqui e nao seja diretiva de preproc, vai ser gerado erro de qualquer jeito.
    //Isso está certo?

    // - a definicao de label sera escrita na mesma linha no arquivo preproc

    if (tokens->size() == 1)
    {
        //nao precisa escrever no arquivo preproc caso dê erro aqui
        if (getTokenType((*tokens)[0]) == DEF_LABEL)
        {
            getLabelDefInNextLine(tokens, rawFile);
        }
    }
}
