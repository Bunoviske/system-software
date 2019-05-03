#ifndef PASSAGE2_INTERFACE
#define PASSAGE2_INTERFACE

#include "../../utils/fileReader.hpp"
#include "../../utils/fileWriter.hpp"

class Passage2Interface {

public:

    Passage2Interface(){};//construtor
    ~Passage2Interface(){};

    //metodos virtuais puros -> necessario implementar na classe derivada
    virtual void run(FileReader* rawFile, FileWriter* preprocFile) = 0;
    virtual FileReader* getFileReader(string filename) = 0;
    virtual FileWriter* getFileWriter(string filename) = 0;
};

#endif