#ifndef PREPROC
#define PREPROC

#include "../interfaces/preprocInterface.hpp"
#include "../../utils/tablesService.hpp"
#include "../errorAnalysis/errorService.hpp"


class PreProcessing : public PreProcessingInterface {

public:

    PreProcessing(){}; //construtor
    ~PreProcessing(){}; //destrutor
    void run(FileReader* rawFile, FileWriter* preprocFile);
    FileReader* getFileReader(string filename);
    FileWriter* getFileWriter(string filename);

private:

    ErrorService errorService;   
    
    string parseWords(int lineNumber, vector<string>& words);
    vector<string>& getTokensOfLine(string line);

};

#endif