#include "tablesService.hpp"

TablesService tables; //instancia global unica da tablesService 

/****************** DIRETIVAS *******************/

int TablesService::getDirectiveOperands(string directive){
    return directivesTable[directive].operands;
}
int TablesService::getDirectiveSize(string directive){
    return directivesTable[directive].size;
}
bool TablesService::isDirectiveInTable(string directive){
    return !(directivesTable.find(directive) == directivesTable.end());
}

/****************** INSTRUCOES *******************/


int TablesService::getInstructionOperands(string instruction){
    return instructionsTable[instruction].operands;
}
int TablesService::getInstructionCode(string instruction){
    return instructionsTable[instruction].code;
}
int TablesService::getInstructionSize(string instruction){
    return instructionsTable[instruction].size;
}
bool TablesService::isInstructionInTable(string instruction){
    return !(instructionsTable.find(instruction) == instructionsTable.end());
}

/****************** SIMBOLOS *******************/

int TablesService::getSymbolAddress(string symbol){
    return symbolsTable[symbol];
}

void TablesService::setSymbolTable(string symbol, int address){
    symbolsTable[symbol] = address;
}

bool TablesService::isSymbolInTable(string symbol){
    return !(symbolsTable.find(symbol) == symbolsTable.end());
}

/****************** MACRO *******************/

int TablesService::getMacroArguments(string macroName){
    return mnt[macroName].arguments;
}
string TablesService::getMacroAssemblyCode(string macroName){
    return mdt[ mnt[macroName].mdtLine ];
}
void TablesService::setMacroAtTable(string macroName, int arguments, string assemblyCode){
    setMacroAtMnt(macroName, arguments);
    setMacroAtMdt(assemblyCode);
}

bool TablesService::isMacroInTable(string macroName){
    return !(mnt.find(macroName) == mnt.end());
}

void TablesService::setMacroAtMnt(string macroName, int arguments){
    mnt[macroName] =  Mnt(arguments, numMacros);
}
void TablesService::setMacroAtMdt(string assemblyCode){
    mdt[numMacros] = assemblyCode;
    numMacros++;
}