#include "semanticAnalysis.hpp"

//inicializacao das variaveis estaticas. Necessario ocorrer aqui, senao da erro
int SemanticAnalyser::lineSectionText = 0;
int SemanticAnalyser::lineSectionData = 0;

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
    else
    { // secao de texto. Essa logica só vale para diretivas, pois as q sao analisadas na secao de texto
        // estao no preproc. Nesse caso, pode ser que lineSectionData ainda seja 0 e vá ser definido depois
        // Nas outras passagens, não é necessário determinar a linha das secoes
        if (currentLine > lineSectionText && lineSectionText != 0 &&
            (currentLine < lineSectionData || lineSectionData == 0))
            return true;
        else
        {
            throwError("Diretiva fora da secao de texto");
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
    if (tables.isLabelInEquTable(label)){
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
    if (tables.isMacroInTable(macroName)){ 
        throwError("Label ja definido como MACRO");
        return true;
    }
    return false;
}