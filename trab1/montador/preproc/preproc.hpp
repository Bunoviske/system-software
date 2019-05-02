#ifndef PREPROC
#define PREPROC

#include "../preprocInterface.hpp"
#include "../../utils/tablesService.hpp"

class PreProcessing : public PreProcessingInterface {

public:

    PreProcessing(){}; //construtor
    ~PreProcessing(){};
    void run(FileReader* rawFile, FileWriter* preprocFile);
    FileReader* getFileReader(string filename);
    FileWriter* getFileWriter(string filename);

private:


};

#endif