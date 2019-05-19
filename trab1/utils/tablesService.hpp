#ifndef TABLES_SERVICE
#define TABLES_SERVICE

#include "tables.hpp"

class TablesService : private Tables {

public:

    TablesService() : Tables() {}; //construtor
    ~TablesService() { }

    int getDirectiveOperands(string directive);
    int getDirectiveSize(string directive);
    bool isDirectiveInTable(string directive);

    int getInstructionOperands(string instruction);
    int getInstructionCode(string instruction);
    int getInstructionSize(string instruction);
    bool isInstructionInTable(string instruction);

    bool isSymbolInTable(string symbol);
    int getSymbolAddress(string symbol);
    void setSymbolTable(string symbol, int address);
    int getNextSymbolAddress(string symbol);

    bool isLabelInEquTable(string label);
    map<string,string> getEquTable();
    void setEquTable(string label, string value);

    bool isMacroInTable(string macroName);
    int getMacroArguments(string macroName);
    string getMacroAssemblyCode(string macroName);
    void setMacroAtTable(string macroName, int arguments, string assemblyCode);


private:

    int numMacros = 0;
    void setMacroAtMnt(string macroName, int arguments);
    void setMacroAtMdt(string assemblyCode);

};

extern TablesService tables; //classe so tem uma instancia global

#endif


    /*
    * TESTE DA CLASSE TABLES SERVICE
    */

    // cout << tables.getDirectiveOperands("CONST") << endl;
    // cout << tables.getDirectiveSize("CONST") << endl;
    // cout << tables.isDirectiveInTable("CONST") << endl;
    // cout << tables.isDirectiveInTable("OLA") << endl;

    // cout << tables.getInstructionOperands("OUTPUT") << endl;
    // cout << tables.getInstructionSize("OUTPUT") << endl;
    // cout << tables.getInstructionCode("OUTPUT") << endl;

    // cout << tables.isSymbolInTable("OUTPUT") << endl;
    // tables.setSymbolTable("OUTPUT", 10);
    // cout << tables.getSymbolAddress("OUTPUT") << endl;
    // cout << tables.isSymbolInTable("OUTPUT") << endl;

    // cout << tables.isMacroInTable("ola") << endl;
    // tables.setMacroAtTable("ola", 2, "bla\nbla\nbla");
    // cout << tables.getMacroArguments("ola") << endl;
    // cout << tables.getMacroAssemblyCode("ola") << endl;
    // cout << tables.isMacroInTable("ola") << endl;
