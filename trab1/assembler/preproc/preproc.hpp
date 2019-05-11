#ifndef PREPROC
#define PREPROC

#include "../interfaces/preprocInterface.hpp"
#include "../../utils/tablesService.hpp"
#include "../errorAnalysis/errorService.hpp"


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
    string preprocLine;
    int lineNumber;
    vector<string> preprocTokens; //contem diretivas e rotulos declarados que sao necessarios analisar no preproc
 
    void analyseDefLabel(vector<string> &tokens, FileReader *rawFile);
    void analyseMacroCall(vector<string> &tokens);
    void analyseDirective(vector<string> &tokens, FileReader *rawFile);

    void parseTokens(vector<string>& tokens, FileReader *rawFile);
    vector<string> getTokensOfLine(string line);
    bool tokensNeedPreproc(vector<string>& tokens);
    void changeEquValues(vector<string>* tokens);
    void assemblePreprocLine(vector<string> &tokens);

};

#endif