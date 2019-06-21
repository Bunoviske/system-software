#ifndef TABLES_SERVICE
#define TABLES_SERVICE

#include "tables.hpp"

struct comp
{
    template <typename T>
    bool operator()(const T &l, const T &r) const
    {
        if (l.second != r.second)
            return l.second < r.second;

        return l.first < r.first;
    }
};

class TablesService : private Tables
{

public:
    TablesService() : Tables(){}; //construtor
    ~TablesService() {}

    int getDirectiveOperands(string directive);
    int getDirectiveSize(string directive);
    bool isDirectiveInTable(string directive);

    int getInstructionOperands(string instruction);
    int getInstructionCode(string instruction);
    int getInstructionSize(string instruction);
    bool isInstructionInTable(string instruction);

    bool isLabelInEquTable(string label);
    map<string, string> getEquTable();
    void setEquTable(string label, string value);

private:
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
