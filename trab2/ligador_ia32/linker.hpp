#ifndef LINKER
#define LINKER

#include "utils.hpp"
#include <map>

#define TEXT_SECTION 1
#define DATA_SECTION 2

class Linker
{

public:
    Linker()
    {
        //TODO - Adicionar variacao das instrucoes
        instructionsTable["ADD"] = 0x00000000;
        instructionsTable["SUB"] = 0x00000000;
        instructionsTable["IMUL"] = 0x00000000;
        instructionsTable["IDIV"] = 0x00000000;
        instructionsTable["MOV"] = 0x00000000;
        instructionsTable["JMP"] = 0x00000000;
        instructionsTable["CMP"] = 0x00000000;
        instructionsTable["JL"] = 0x00000000;
        instructionsTable["JG"] = 0x00000000;
        instructionsTable["JE"] = 0x00000000;
        instructionsTable["PUSH"] = 0x00000000;
        instructionsTable["POP"] = 0x00000000;
        instructionsTable["INT"] = 0x00000000;
        instructionsTable["CALL"] = 0x00000000;

        symbolAddress = 0;
        currentSection = 0;

    };           //construtor
    ~Linker(){}; //destrutor

    void run(string &text, string &data, string filename);

private:
    void firstPassage(string &data, string filename);
    void secondPassage(string &text, string &data, string filename);

    void writeTextSection(string &text, string &data, vector<string> tokens);
    void writeDataSection(string &data, vector<string> tokens);
    void writeBinaryInstructions(string &text, string &data, vector<string> tokens);
    bool setCurrentSection(vector<string> tokens);

    int getAddressDisplacement(string token);
    string getLabelName(string token);
    void searchSymbol(string &data, vector<string> tokens);
    bool isSymbolInTable(string symbol);
    bool isInstructionInTable(string instruction);

    uint32_t symbolAddress, currentSection;
    map<string, uint32_t> instructionsTable;
    map<string, uint32_t> symbolsTable; //string representa o rotulo e inteiro represento o endereço
};

#endif
