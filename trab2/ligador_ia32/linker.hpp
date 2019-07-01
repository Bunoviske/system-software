#ifndef LINKER
#define LINKER

#include "utils.hpp"
#include <map>

#define TEXT_SECTION 1
#define DATA_SECTION 2

class Linker
{
private:
    void firstPassage(string &data, string filename);
    void secondPassage(string &text, string &data, string filename);

    void writeTextSection(string &text, string &data, vector<string> tokens);
    void writeDataSection(string &data, vector<string> tokens);
    void writeBinaryInstructions(string &text, string &data, vector<string> tokens);
    bool setCurrentSection(vector<string> tokens);

    string getInstructionOpcode(vector<string> tokens, int *nextTokenToProcess);
    void getArgumentsBinaryCode(string &text, string &data, vector<string> tokens, bool isJump);
    int getAddressDisplacement(string token);
    string getLabelName(string token);
    void searchSymbol(string &data, vector<string> tokens);
    bool isSymbolInTable(string symbol);
    bool isInstructionInTable(string instruction);
    bool isJumpInstruction(string instruction);
    uint32_t getInstructionIncrementSize(string instruction);

    uint32_t textSymbolAddress, currentSection,dataSymbolAddress;
    map<string, string> instructionsTable;
    map<string, string> opcodesTable;
    map<string, uint32_t> dataSymbolsTable;
    map<string, uint32_t> textSymbolsTable; //string representa o rotulo e inteiro represento o endereço

public:
    void run(string &text, string &data, string filename);

    Linker()
    {
        textSymbolAddress = 0;
        currentSection = 0;
        dataSymbolAddress = 0;

        /**
         * INSTRUCTIONS TABLE
         * 
         **/
        instructionsTable["ADD"] = "0x00000000";
        instructionsTable["SUB"] = "0x00000000";
        instructionsTable["IMUL"] = "0x00000000";
        instructionsTable["IDIV"] = "0x00000000";
        instructionsTable["MOV"] = "0x00000000";
        instructionsTable["JMP"] = "0x00000000";
        instructionsTable["CMP"] = "0x00000000";
        instructionsTable["JL"] = "0x00000000";
        instructionsTable["JG"] = "0x00000000";
        instructionsTable["JE"] = "0x00000000";
        instructionsTable["PUSH"] = "0x00000000";
        instructionsTable["POP"] = "0x00000000";
        instructionsTable["INT"] = "0x00000000";

        //instrucoes das funcoes implementadas em INPUT e OUTPUT
        instructionsTable["CALL"] = "0x00000000";
        instructionsTable["RET"] = "0x00000000";
        instructionsTable["JGE"] = "0x00000000";
        instructionsTable["INC"] = "0x00000000";
        instructionsTable["JNE"] = "0x00000000";
        instructionsTable["MOVZX"] = "0x00000000";
        instructionsTable["JB"] = "0x00000000";
        instructionsTable["JBE"] = "0x00000000";
        instructionsTable["JA"] = "0x00000000";
        instructionsTable["DEC"] = "0x00000000";

        /**
         * OPCODES TABLE
         * 
         **/
        opcodesTable["MOV EAX"] = hex2ascii(0xb8, false);
        opcodesTable["MOV EBX"] = hex2ascii(0xbb, false);
        opcodesTable["MOV ECX"] = hex2ascii(0xb9, false);
        opcodesTable["MOV EDX"] = hex2ascii(0xba, false);
        opcodesTable["INT 0X80"] = hex2ascii(0xcd80, false);


    };           //construtor
    ~Linker(){}; //destrutor
};

#endif
