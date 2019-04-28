#include "tabelas.hpp"

Tables tables;

Tables::Tables(){

    directivesTable["SECTION"] = DirectiveTable(1,0);
    directivesTable["SPACE"] = DirectiveTable(-1,-1); //-1 significa que operandos pode ser variavel e tamanho tb
    directivesTable["CONST"] = DirectiveTable(1,1); 
    directivesTable["EQU"] = DirectiveTable(1,0); 
    directivesTable["IF"] = DirectiveTable(1,0); 
    directivesTable["MACRO"] = DirectiveTable(0,0); 
    directivesTable["END"] = DirectiveTable(0,0); 

    instructionsTable["ADD"] = InstructionTable(1,1,2);
    instructionsTable["SUB"] = InstructionTable(1,2,2);
    instructionsTable["MULT"] = InstructionTable(1,3,2);
    instructionsTable["DIV"] = InstructionTable(1,4,2);
    instructionsTable["JMP"] = InstructionTable(1,5,2);
    instructionsTable["JMPN"] = InstructionTable(1,6,2);
    instructionsTable["JMPP"] = InstructionTable(1,7,2);
    instructionsTable["JMPZ"] = InstructionTable(1,8,2);
    instructionsTable["COPY"] = InstructionTable(2,9,3);
    instructionsTable["LOAD"] = InstructionTable(1,10,2);
    instructionsTable["STORE"] = InstructionTable(1,11,2);
    instructionsTable["INPUT"] = InstructionTable(1,12,2);
    instructionsTable["OUTPUT"] = InstructionTable(1,13,2);
    instructionsTable["STOP"] = InstructionTable(0,14,1);

}