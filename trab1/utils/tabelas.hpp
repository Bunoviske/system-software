#ifndef TABELAS
#define TABELAS

#include <iostream>
#include <map>

using namespace std;

struct InstructionTable {

    int operands, code, size;

    InstructionTable(int operands, int code, int size){
        this->operands = operands;
        this->code = code;
        this->size = size;
    }
    InstructionTable(){}

};

struct DirectiveTable {

    int operands, size;

    DirectiveTable(int operands, int size){
        this->operands = operands;
        this->size = size;
    }
    DirectiveTable(){}
};

struct Mnt {
    
    int arguments, mdtLine;

    Mnt(int arguments, int mdtLine){
        this->arguments = arguments;
        this->mdtLine = mdtLine;
    }
    Mnt(){}

};

class Tables {

public:

    Tables(); //construtor
    ~Tables() { }

    int getDirectiveOperands(string directive);
    int getDirectiveSize(string directive);

    int getInstructionOperands(string instruction);
    int getInstructionCode(string instruction);
    int getInstructionSize(string instruction);

    int getSymbolAddress(string symbol);
    void setSymbolTable(string symbol, int address);

    int getMacroArguments(string macroName);
    string getMacroAssemblyCode(string macroName);
    void setMacroAtTable(int arguments, int length, string assemblyCode);


private:

    void setMacroAtMnt(int arguments);
    void setMacroAtMdt(int length, string assemblyCode);

    map<string,DirectiveTable> directivesTable;
    map<string,InstructionTable> instructionsTable;
    map<string,int> symbolsTable; //string representa o rotulo e inteiro represento o endereço
    map<string,Mnt> mnt;
    map<int,string> mdt; //primeiro numero se refere a "linha" da tabela e a string é a macro

};

extern Tables tables;

#endif