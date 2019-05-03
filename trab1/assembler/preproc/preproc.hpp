#ifndef PREPROC
#define PREPROC

#include "../interfaces/preprocInterface.hpp"
#include "../../utils/tablesService.hpp"
#include "../errorAnalysis/errorService.hpp"

class PreProcessing : public PreProcessingInterface {

public:

    PreProcessing(){}; //construtor
    ~PreProcessing(){};
    void run(FileReader* rawFile, FileWriter* preprocFile);
    FileReader* getFileReader(string filename);
    FileWriter* getFileWriter(string filename);

private:

    ErrorService errorService;   
    


};

#endif