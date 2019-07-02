#ifndef LINKER
#define LINKER

#include "utils.hpp"
#include <map>

#define TEXT_SECTION 1
#define DATA_SECTION 2

//distancia das funcoes até a instrucao seguinte a STOP (igual ao endereco do label LERINTEIRO)
#define LER_INTEIRO_OFFSET 0x00
#define ESCREVER_INTEIRO_OFFSET 0xab
#define LER_CHAR_OFFSET 0x14a
#define ESCREVER_CHAR_OFFSET 0x191
#define LER_STRING_OFFSET 0x1d4
#define ESCREVER_STRING_OFFSET 0x251
#define LER_HEXA_OFFSET 0x2aa
#define ESCREVER_HEXA_OFFSET 0x361

class Linker
{
private:
    //passagem 1
    void firstPassage(string &data, string filename);
    void writeDataSection(string &data, vector<string> tokens);
    void searchSymbol(string &data, vector<string> tokens);
    uint32_t getInstructionIncrementSize(vector<string> tokens, int instructionPos);

    //passagem 2
    void secondPassage(string &text, string filename);
    string getBinaryFunctions();
    void writeTextSection(string &text, vector<string> tokens);
    void writeBinaryInstructions(string &text, vector<string> tokens);
    string getInstructionOpcode(vector<string> tokens);
    void getArgumentsBinaryCode(string &text, vector<string> tokens, bool isJump);
    int getAddressDisplacement(string token);
    string getLabelName(string token);
    uint32_t getAssemblyFunctionDistance(string function);

    //ambas
    bool setCurrentSection(vector<string> tokens);
    bool isSymbolInTable(string symbol);
    bool isInstructionInTable(string instruction);
    bool isInTextSymbolTable(string symbol);
    bool isJumpInstruction(string instruction);

    uint32_t textSymbolAddress, currentSection, dataSymbolAddress, finalMainAddress;
    map<string, uint32_t> instructionsTable; //segundo argumento é o tamanho da instrucao
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
        finalMainAddress = 0;

        //preenche tabela de simbolos com as funcoes criadas. Esses enderecos nao importam para a logica
        textSymbolsTable["LERINTEIRO"] = 0;
        textSymbolsTable["ESCREVERINTEIRO"] = 0;
        textSymbolsTable["LERCHAR"] = 0;
        textSymbolsTable["ESCREVERCHAR"] = 0;
        textSymbolsTable["LERHEXA"] = 0;
        textSymbolsTable["ESCREVERHEXA"] = 0;
        textSymbolsTable["LERSTRING"] = 0;
        textSymbolsTable["ESCREVERSTRING"] = 0;

        /**
         * INSTRUCTIONS TABLE
         * 
         **/
        instructionsTable["ADD"] = 6;
        instructionsTable["SUB"] = 6;
        instructionsTable["IMUL"] = 6;
        instructionsTable["IDIV"] = 6;
        instructionsTable["MOV"] = 5; //considerando apenas MOV EAX e MOV EDX que aparecem no assembly inventado
        instructionsTable["JMP"] = 2;
        instructionsTable["CMP"] = 3;
        instructionsTable["JL"] = 2;
        instructionsTable["JG"] = 2;
        instructionsTable["JE"] = 2;
        instructionsTable["POP"] = 1; //Considerando sempre o caso de POP EAX
        instructionsTable["PUSH EAX"] = 1;
        instructionsTable["PUSH"] = 5;    //PUSH LABEL
        instructionsTable["PUSH []"] = 6; //PUSH [LABEL]
        instructionsTable["INT"] = 2;
        instructionsTable["CALL"] = 5;

        //instrucoes das funcoes implementadas em INPUT/OUTPUT
        // instructionsTable["RET"] = "0x00000000";
        // instructionsTable["JGE"] = "0x00000000";
        // instructionsTable["INC"] = "0x00000000";
        // instructionsTable["JNE"] = "0x00000000";
        // instructionsTable["MOVZX"] = "0x00000000";
        // instructionsTable["JB"] = "0x00000000";
        // instructionsTable["JBE"] = "0x00000000";
        // instructionsTable["JA"] = "0x00000000";
        // instructionsTable["DEC"] = "0x00000000";

        /**
         * OPCODES TABLE         
         **/

        //EAX
        opcodesTable["MOV EAX"] = hex2ascii(0xb8, false);
        opcodesTable["MOV EAX []"] = hex2ascii(0xa1, false);
        opcodesTable["MOV [] EAX"] = hex2ascii(0xa3, false);
        opcodesTable["ADD EAX []"] = hex2ascii(0x0305, false);
        opcodesTable["SUB EAX []"] = hex2ascii(0x2b05, false);
        opcodesTable["IMUL []"] = hex2ascii(0xf72d, false);
        opcodesTable["IDIV []"] = hex2ascii(0xf73d, false);
        opcodesTable["CMP EAX 0"] = hex2ascii(0x83f800, false);
        opcodesTable["PUSH EAX"] = hex2ascii(0x50, false);
        opcodesTable["POP EAX"] = hex2ascii(0x58, false);

        //EBX
        opcodesTable["MOV EBX"] = hex2ascii(0xbb, false);
        opcodesTable["MOV EBX []"] = hex2ascii(0x8b1d, false);
        opcodesTable["MOV [] EBX"] = hex2ascii(0x891d, false);

        //ECX
        opcodesTable["MOV ECX"] = hex2ascii(0xb9, false);
        opcodesTable["MOV ECX []"] = hex2ascii(0x8b0d, false);
        opcodesTable["MOV [] ECX"] = hex2ascii(0x890d, false);

        //EDX
        opcodesTable["MOV EDX"] = hex2ascii(0xba, false);
        opcodesTable["MOV EDX []"] = hex2ascii(0x8b15, false);
        opcodesTable["MOV [] EDX"] = hex2ascii(0x8915, false);

        //SHORT JUMP
        //INSTRUCOES QUE TEM JUMP RELATIVO
        opcodesTable["JMP"] = hex2ascii(0xeb, false);  //jump de 8 bits
        opcodesTable["JG"] = hex2ascii(0x7f, false);   //jump de 8 bits
        opcodesTable["JL"] = hex2ascii(0x7c, false);   //jump de 8 bits
        opcodesTable["JE"] = hex2ascii(0x74, false);   //jump de 8 bits
        opcodesTable["CALL"] = hex2ascii(0xe8, false); //jump de 32 bits

        //OTHERS
        opcodesTable["PUSH"] = hex2ascii(0x68, false);      //PUSH LABEL
        opcodesTable["PUSH []"] = hex2ascii(0xff35, false); //PUSH [LABEL]
        opcodesTable["INT 0X80"] = hex2ascii(0xcd80, false);

    };           //construtor
    ~Linker(){}; //destrutor
};

#endif
