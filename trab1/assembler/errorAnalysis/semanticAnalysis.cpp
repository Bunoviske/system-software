#include "semanticAnalysis.hpp"

//inicializacao das variaveis estaticas. Necessario ocorrer aqui, senao da erro
int SemanticAnalyser::lineSectionText = 0;
int SemanticAnalyser::lineSectionData = 0;
int SemanticAnalyser::positionSectionText = 0;
int SemanticAnalyser::positionSectionData = 0;

//TODA VEZ QUE CHAMAR LEXICO, TEM QUE CHAMAR A FUNCAO lexical.setCurrentLine();

bool SemanticAnalyser::isDirectiveInCorrectSection(string directive)
{
    if (directive == "CONST" || directive == "SPACE")
    { //secao de dados
        if (currentLine > lineSectionData && lineSectionData != 0)
            return true;
        else
        {
            throwError("Diretiva fora da secao de dados");
            return false;
        }
    }
    else if (directive == "EQU") //antes da secao de dados e de texto
        if (lineSectionData == 0 && lineSectionText == 0)
            return true;
        else
        {
            throwError("Diretiva equ nao esta antes da secao de texto e dados");
            return false;
        }
    else if (directive == "MACRO")
    { // secao de texto. Essa logica só vale para diretivas, pois as q sao analisadas na secao de texto
        // estao no preproc. Nesse caso, pode ser que lineSectionData ainda seja 0 e vá ser definido depois
        // Nas outras passagens, não é necessário determinar a linha das secoes
        if (currentLine > lineSectionText && lineSectionText != 0 &&
            (currentLine < lineSectionData || lineSectionData == 0))
            return true;
        else
        {
            throwError("Diretiva macro fora da secao de texto");
            return false;
        }
    }
}

bool SemanticAnalyser::isInstructionInCorrectSection()
{
    //instrucoes so sera avaliadas na primeira e segunda passagem. As variaveis lineSectionText e Data ja estao definidas
    // Porem, secao de dados nao é obg!
    if (currentLine > lineSectionText && lineSectionText != 0 &&
        (currentLine < lineSectionData || lineSectionData == 0))
        return true;
    else
    {
        throwError("Instrucao fora da secao de texto");
        return false;
    }
}

void SemanticAnalyser::setSectionPosition(string section, int position)
{
    if (section == "TEXT")
    {
        positionSectionText = position;
    }
    else if (section == "DATA")
    {
        positionSectionData = position;
    }
}

void SemanticAnalyser::setSectionLine(string section)
{
    if (section == "TEXT")
        lineSectionText = currentLine;
    else if (section == "DATA")
        lineSectionData = currentLine;
    else
    {
        throwError("Secao inexistente");
    }
}

void SemanticAnalyser::checkSectionOrder()
{
    if (lineSectionText == 0)
    {
        throwError("Secao de texto nao existe");
    }
    else if (lineSectionData < lineSectionText && lineSectionData != 0)
    {
        throwError("Secao de dados antes da secao de texto");
    }
}

/*********** preproc semantic errors **************/

//essa funcao é um caso especial pois lida com EQU. Como as variaveis EQU sao substituidas antes de preproc uma linha,
//a string label que é recebida como parâmetro deve ser um número caso ela exista na tabela (ja foi substituida no preproc)
bool SemanticAnalyser::isLabelInEquTable(string label)
{
    if (lexical.isNumerical(label))
        return true;
    throwError("Label nao foi definido como EQU");
    return false;
}

bool SemanticAnalyser::isEquAlreadyDefined(string label)
{
    if (tables.isLabelInEquTable(label))
    {
        throwError("Label ja definido como EQU");
        return true;
    }
    return false;
}
bool SemanticAnalyser::isMacroInTable(string macroName)
{
    if (tables.isMacroInTable(macroName))
        return true;
    throwError("Label nao foi definido como MACRO");
    return false;
}

bool SemanticAnalyser::isMacroAlreadyDefined(string macroName)
{
    if (tables.isMacroInTable(macroName))
    {
        throwError("Label ja definido como MACRO");
        return true;
    }
    else if (tables.isLabelInEquTable(macroName))
    {
        throwError("Label ja definido como EQU");
        return true;
    }
    return false;
}

//funcao checa se o numero de argumentos é compativel e se a macro foi definida
bool SemanticAnalyser::isMacroCallCorrect(vector<string> tokens, int numMacroArgs)
{

    if (isMacroInTable(tokens[0]))
    {
        int numArguments = tables.getMacroArguments(tokens[0]);
        if (numArguments == numMacroArgs)
            return true;

        throwError("Numero errado de argumentos da macro");
    }
    return false;
}

/************* 1st passage semantic errors **************/

bool SemanticAnalyser::isSymbolAlreadyDefined(string label)
{

    if (tables.isSymbolInTable(label))
    {
        throwError("Label ja definido anteriormente");
        return true;
    }

    if (tables.isMacroInTable(label))
    {
        throwError("Label ja definido como macro");
        return true;
    }

    if (tables.isLabelInEquTable(label))
    {
        throwError("Label ja definido com a diretiva EQU");
        return true;
    }

    return false;
}

bool SemanticAnalyser::isOperation(string token)
{

    lexical.setLineNumber(this->currentLine);
    int tokenType = lexical.getTokenType(token);

    if ((tokenType == INSTRUCTION) || (tokenType == DIRECTIVE))
    {
        return true;
    }
    else
    {
        throwError("Operacao invalida");
        return false;
    }
}

/************* 2nd passage semantic errors **************/

bool SemanticAnalyser::isSymbolDefined(string label)
{
    if (!tables.isSymbolInTable(label))
    {
        throwError("Label nao definido");
        return false;
    }
    return true;
}

bool SemanticAnalyser::isSymbolOffsetCorrect(string label, int offset)
{
    int currentAddress, nextAddress;
    currentAddress = tables.getSymbolAddress(label);
    nextAddress = tables.getNextSymbolAddress(label); //sempre vai ter um proximo offset (0LastSymbolAux)

    if (currentAddress + offset < nextAddress)
    {
        return true;
    }

    throwError("Acesso a posicao nao reservada para esse label");
    return false;
}

//so se entra nessa funcao se souber que a sintaxe esta certa, entao pode acessar vetor sem verificar
bool SemanticAnalyser::checkInstructionSemantic(vector<string> tokens)
{
    if ((tokens[0] == "JMP") || (tokens[0] == "JMPN") || (tokens[0] == "JMPP") || (tokens[0] == "JMPZ"))
    {
        return checkJumpToCorrectSection(tokens);
    }
    else if (tokens[0] == "DIV")
    {
        if (tables.isInConstTable(tokens[1]))
        {
            string constNum = tables.getConstTable(tokens[1]);
            int num;
            if (lexical.isHexadecimalNumber(constNum))
            {
                constNum = constNum.substr(2, constNum.size());
                num = stoi(constNum, NULL, 16);
            }
            else
            {
                num = stoi(constNum, NULL, 10);
            }
            if (num == 0)
            {
                throwError("Divisao por valor constante igual a zero");
                return false;
            }
        }
    }
    else if (tokens[0] == "STORE" || tokens[0] == "INPUT")
    {
        if (tables.isInConstTable(tokens[1]))
        { //sintaxe ja foi verificada, entao pode acessar
            throwError("Nao pode alterar valor constante");
            return false;
        }
    }
    else if (tokens[0] == "COPY")
    {
        int numLabel = 0;
        for (size_t i = 1; i < tokens.size(); i++)
        {
            int tokenType = lexical.getTokenType(tokens[i]);
            if (tokenType == COPY_ARGUMENT || tokenType == LABEL)
                numLabel++;

            if (numLabel == 2 && tables.isInConstTable(tokens[i])) 
            //sintaxe vai estar correta, entao ultimo label pode ser acessado por token[i] diretamente
            {
                throwError("Nao pode alterar valor constante");
                return false;
            }
        }
    }
    return true;
}

bool SemanticAnalyser::checkJumpToCorrectSection(vector<string> tokens)
{
    if (tables.getSymbolAddress(tokens[1]) < positionSectionData)
    {
        return true;
    }
    throwError("Jump para a secao de dados");
    return false;
}
