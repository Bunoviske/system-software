#ifndef SERVICE_INTERFACE
#define SERVICE_INTERFACE

#include "../../utils/fileReader.hpp"
#include "../../utils/fileWriter.hpp"

class ServiceInterface {

public:

    ServiceInterface(){};//construtor
    ~ServiceInterface(){};

    //metodos virtuais puros -> necessario implementar na classe derivada
    virtual void run(FileReader* rawFile, FileWriter* preprocFile) = 0;
    virtual FileReader* getFileReader(string filename) = 0;
    virtual FileWriter* getFileWriter(string filename) = 0;
};

#endif