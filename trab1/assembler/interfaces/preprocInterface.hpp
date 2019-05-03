#ifndef PREPROC_INTERFACE
#define PREPROC_INTERFACE

#include "../../utils/fileReader.hpp"
#include "../../utils/fileWriter.hpp"

class PreProcessingInterface {

public:

    PreProcessingInterface(){};//construtor
    ~PreProcessingInterface(){};

    //metodos virtuais puros -> necessario implementar na classe derivada
    virtual void run(FileReader* rawFile, FileWriter* preprocFile) = 0;
    virtual FileReader* getFileReader(string filename) = 0;
    virtual FileWriter* getFileWriter(string filename) = 0;
};

#endif