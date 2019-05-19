#ifndef SEMANTIC_ANALYSIS
#define SEMANTIC_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"
#include "lexicalAnalysis.hpp"

class SemanticAnalyser : public BaseAnalyser
{

public:

    static int lineSectionText;
    static int lineSectionData;
    static int positionSectionText;
    static int positionSectionData;
    
    //nao mudam para as varias instancias da classe de analise semantica.

    SemanticAnalyser(){
        lexical.setErrorListener(new LexicalListener());
    }; //construtor
    ~SemanticAnalyser(){};


    bool isDirectiveInCorrectSection(string directive);
    bool isInstructionInCorrectSection();
    void setSectionPosition(string section, int position);
    void setSectionLine(string section); //currentLine é usada para determinar a linha da secao atual


    void checkSectionOrder(); //TODO - chamar essa funcao antes da segunda passagem!

    /*********** preproc semantic errors **************/
    bool isLabelInEquTable(string label);
    bool isEquAlreadyDefined(string label);
    bool isMacroInTable(string macroName);
    bool isMacroCallCorrect(vector<string> tokens);
    bool isMacroAlreadyDefined(string macroName);


    /********* 1st passage semantic errors ***********/
    bool isSymbolAlreadyDefined(string label);
    bool isOperation(string token);

    /********* 2nd passage semantic errors *******/


    bool isSymbolDefined(string label);
    bool isSymbolOffsetCorrect(string label, int offset);
    bool checkInstructionSemantic(vector<string> tokens);

private:

    LexicalAnalyser lexical;
    bool checkJumpToCorrectSection(vector<string> tokens);

};

#endif
