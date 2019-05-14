#ifndef PREPROC
#define PREPROC

#include "macroProcessing.hpp"

class PreProcessing : public PreProcessingInterface
{

public:
    PreProcessing()
    {
        preprocTokens.push_back("EQU");
        preprocTokens.push_back("MACRO");
        preprocTokens.push_back("IF");
        preprocTokens.push_back("SECTION");
        skipNextMacroLine = false;
        isMacroLine = false;

    }; //construtor

    ~PreProcessing(){}; //destrutor
    void run(FileReader *rawFile, FileWriter *preprocFile);
    FileReader *getFileReader(string filename);
    FileWriter *getFileWriter(string filename);

private:
    ErrorService errorService;
    MacroProcessing macroProcessing;
    string preprocLine;
    int lineNumber;
    bool isMacroLine, skipNextMacroLine; //auxilia na logica do IF caso ele esteja dentro de uma MACRO expandida
    map<string, int> macroNumberOfLines; //variavel auxilia na contagem das linhas de uma macro
    vector<string> preprocTokens;        //contem diretivas e rotulos declarados que sao necessarios analisar no preproc

    void analyseDefLabel(vector<string> &tokens, FileReader *rawFile);
    void analyseMacroCall(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile);
    void analyseDirective(vector<string> &tokens, FileReader *rawFile);

    void parseCodeLine(string line, FileReader *rawFile, FileWriter *preprocFile);
    void parseTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile);
    void assemblePreprocLine(vector<string> &tokens);
    bool tokensNeedPreproc(vector<string> &tokens);
    void changeEquValues(vector<string> *tokens);
};

#endif