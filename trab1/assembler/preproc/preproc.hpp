#ifndef PREPROC
#define PREPROC

#include "macroProcessing.hpp"

class PreProcessing : public PreProcessingInterface {

public:

    PreProcessing(){
        preprocTokens.push_back("EQU");
        preprocTokens.push_back("MACRO");
        preprocTokens.push_back("IF");
        preprocTokens.push_back("SECTION");
    }; //construtor
    ~PreProcessing(){}; //destrutor
    void run(FileReader* rawFile, FileWriter* preprocFile);
    FileReader* getFileReader(string filename);
    FileWriter* getFileWriter(string filename);

private:

    ErrorService errorService; 
    MacroProcessing macroProcessing; 
    string preprocLine;
    int lineNumber;
    map<string,int> macroNumberOfLines; //variavel auxilia na contagem das linhas de uma macro
    vector<string> preprocTokens; //contem diretivas e rotulos declarados que sao necessarios analisar no preproc
 
    void analyseDefLabel(vector<string> &tokens, FileReader *rawFile);
    void analyseMacroCall(vector<string> &tokens);
    void analyseDirective(vector<string> &tokens, FileReader *rawFile);

    void parseTokens(vector<string>& tokens, FileReader *rawFile);
    bool tokensNeedPreproc(vector<string>& tokens);
    void changeEquValues(vector<string>* tokens);
    void assemblePreprocLine(vector<string> &tokens);

};

#endif