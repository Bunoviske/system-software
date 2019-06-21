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

void TranslateProcessing::parseCodeLine(string line, FileReader *rawFile, FileWriter *preprocFile)
{
    vector<string> tokens;
    tokens = getTokensOfLine(line); //retorna apenas palavras diferentes de comentarios, \t e \n

    if (tokens.size())
    { //garante que só chama essa funcao se há palavras
        if (!preprocTokens(tokens, rawFile, preprocFile))
        {
            translateTokens(tokens, rawFile, preprocFile);

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

    else if (tokenType == INSTRUCTION) //nesse caso so pode ser chamando MACRO
        analyseInstruction(tokens, rawFile);

    else if (tokenType == DIRECTIVE)
        analyseDirective(tokens, rawFile);

    //assembletranslatedLine(tokens);
}

string TranslateProcessing::getLabel(vector<string> &tokens){
    //label pode ter offset (displacement)
    
    //TODO - COPY e S_OUTPUT/INPUT nao aceita displacement

    string structure = "";
    for (size_t i = 1; i < tokens.size(); i++)
    {
        structure += tokens[i];
    }
    return structure;
    
}

void TranslateProcessing::analyseInstruction(vector<string> &tokens, FileReader *rawFile)
{
    if (tokens[0] == "ADD")
    {
        translatedLine = "ADD EAX,[" + getLabel(tokens) + ']';
    }
    else if (tokens[0] == "SUB")
    {
        translatedLine = "SUB EAX,[" + getLabel(tokens) + ']';
    }
    else if (tokens[0] == "MULT")
    {
        translatedLine = "IMUL DWORD [" + getLabel(tokens) + ']';
    }
    else if (tokens[0] == "DIV")
    {
        translatedLine = "MOV EDX,0\nIDIV DWORD [" + getLabel(tokens) + ']';
    }
    else if (tokens[0] == "JMP")
    {
        translatedLine = "JMP " + tokens[1];
    }
    else if (tokens[0] == "JMPN")
    {
        translatedLine = "CMP EAX,0\nJL " + tokens[1];
    }
    else if (tokens[0] == "JMPP")
    {
        translatedLine = "CMP EAX,0\nJG " + tokens[1];
    }
    else if (tokens[0] == "JMPZ")
    {
        translatedLine = "CMP EAX,0\nJE " + tokens[1];
    }
    else if (tokens[0] == "COPY")
    {
        tokens[1].pop_back(); //tira a virgula
        translatedLine = "PUSH EAX\n MOV EAX,[" + tokens[1] + "]\nMOV [" + tokens[2] + "],EAX\nPOP EAX";
    }
    else if (tokens[0] == "LOAD")
    {
        translatedLine = "MOV EAX,[" + getLabel(tokens) + ']';
    }
    else if (tokens[0] == "STORE")
    {
        translatedLine = "MOV [" + getLabel(tokens) + "],EAX";
    }
    else if (tokens[0] == "STOP")
    {
        translatedLine = "MOV EAX,0\nMOV EBX, 1\nINT 80h";
    }
    else if (tokens[0] == "INPUT")
    {
        translatedLine = "PUSH " + getLabel(tokens);
        translatedLine += "\nCALL " + INPUT_FUNCTION;
    }
    else if (tokens[0] == "OUTPUT")
    {
        translatedLine = "PUSH " + getLabel(tokens);
        translatedLine += "\nCALL " + OUTPUT_FUNCTION;
    }
    else if (tokens[0] == "C_INPUT")
    {
        translatedLine = "PUSH " + getLabel(tokens);
        translatedLine += "\nCALL " + C_INPUT_FUNCTION;
    }
    else if (tokens[0] == "C_OUTPUT")
    {
        translatedLine = "PUSH " + getLabel(tokens);
        translatedLine += "\nCALL " + C_OUTPUT_FUNCTION;
    }
    else if (tokens[0] == "H_INPUT")
    {
        translatedLine = "PUSH " + getLabel(tokens);
        translatedLine += "\nCALL " + H_INPUT_FUNCTION;
    }
    else if (tokens[0] == "H_OUTPUT")
    {
        translatedLine = "PUSH " + getLabel(tokens);
        translatedLine += "\nCALL " + H_OUTPUT_FUNCTION;
    }
    else if (tokens[0] == "S_INPUT")
    {
        tokens[1].pop_back(); //tira a virgula
        translatedLine = "PUSH " + tokens[1] + "\nPUSH " + tokens[2];
        translatedLine += "\nCALL " + S_INPUT_FUNCTION;
    }
    else if (tokens[0] == "S_OUTPUT")
    {
        tokens[1].pop_back(); //tira a virgula
        translatedLine = "PUSH " + tokens[1] + "\nPUSH " + tokens[2];
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
            // TODO - EQU PODE RECEBER NUMERO NEGATIVO NO IF ?????
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
            translatedLine = "SECTION .TEXT\nGLOBAL _start\n_start:";
        else if (tokens[1] == "DATA")
            translatedLine = "SECTION .DATA";
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
