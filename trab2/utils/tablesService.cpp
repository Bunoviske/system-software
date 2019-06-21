#include "tablesService.hpp"

TablesService tables; //instancia global unica da tablesService

/****************** DIRETIVAS *******************/

int TablesService::getDirectiveOperands(string directive)
{
    return directivesTable[directive].operands;
}
int TablesService::getDirectiveSize(string directive)
{
    return directivesTable[directive].size;
}
bool TablesService::isDirectiveInTable(string directive)
{
    return !(directivesTable.find(directive) == directivesTable.end());
}

/****************** INSTRUCOES *******************/

int TablesService::getInstructionOperands(string instruction)
{
    return instructionsTable[instruction].operands;
}
int TablesService::getInstructionCode(string instruction)
{
    return instructionsTable[instruction].code;
}
int TablesService::getInstructionSize(string instruction)
{
    return instructionsTable[instruction].size;
}
bool TablesService::isInstructionInTable(string instruction)
{
    return !(instructionsTable.find(instruction) == instructionsTable.end());
}

/****************** EQU *******************/

bool TablesService::isLabelInEquTable(string label)
{
    return !(equTable.find(label) == equTable.end());
}
map<string, string> TablesService::getEquTable()
{
    return equTable;
}
void TablesService::setEquTable(string label, string value)
{
    equTable[label] = value;
}
