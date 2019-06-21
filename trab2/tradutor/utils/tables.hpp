#ifndef TABLES
#define TABLES

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

struct InstructionTable
{

    int operands, code, size;

    InstructionTable(int operands, int code, int size)
    {
        this->operands = operands;
        this->code = code;
        this->size = size;
    }
    InstructionTable() {}
};

struct DirectiveTable
{

    int operands, size;

    DirectiveTable(int operands, int size)
    {
        this->operands = operands;
        this->size = size;
    }
    DirectiveTable() {}
};

class Tables
{
public:
    Tables() //construtor
    {
        directivesTable["SECTION"] = DirectiveTable(1, 0);
        directivesTable["SPACE"] = DirectiveTable(-1, -1); //-1 significa que operandos pode ser variavel e tamanho tb
        directivesTable["CONST"] = DirectiveTable(1, 1);
        directivesTable["EQU"] = DirectiveTable(1, 0);
        directivesTable["IF"] = DirectiveTable(1, 0);

        instructionsTable["ADD"] = InstructionTable(1, 1, 2);
        instructionsTable["SUB"] = InstructionTable(1, 2, 2);
        instructionsTable["MULT"] = InstructionTable(1, 3, 2);
        instructionsTable["DIV"] = InstructionTable(1, 4, 2);
        instructionsTable["JMP"] = InstructionTable(1, 5, 2);
        instructionsTable["JMPN"] = InstructionTable(1, 6, 2);
        instructionsTable["JMPP"] = InstructionTable(1, 7, 2);
        instructionsTable["JMPZ"] = InstructionTable(1, 8, 2);
        instructionsTable["COPY"] = InstructionTable(2, 9, 3);
        instructionsTable["LOAD"] = InstructionTable(1, 10, 2);
        instructionsTable["STORE"] = InstructionTable(1, 11, 2);
        instructionsTable["INPUT"] = InstructionTable(1, 12, 2);
        instructionsTable["OUTPUT"] = InstructionTable(1, 13, 2);
        instructionsTable["STOP"] = InstructionTable(0, 14, 1);
        instructionsTable["C_INPUT"] = InstructionTable(1, 15, 2);
        instructionsTable["C_OUTPUT"] = InstructionTable(1, 16, 2);
        instructionsTable["H_INPUT"] = InstructionTable(1, 17, 2);
        instructionsTable["H_OUTPUT"] = InstructionTable(1, 18, 2);
        instructionsTable["S_INPUT"] = InstructionTable(2, 19, 3);
        instructionsTable["S_OUTPUT"] = InstructionTable(2, 20, 3);
    };
    ~Tables() {}

protected:

    map<string, DirectiveTable> directivesTable;
    map<string, InstructionTable> instructionsTable;
    map<string, string> equTable; //string representa o rotulo e inteiro represento o numero
};

#endif