#ifndef PASSAGE1_INTERFACE
#define PASSAGE1_INTERFACE

#include "../../utils/fileReader.hpp"

class Passage1Interface {

public:

    Passage1Interface(){};//construtor
    ~Passage1Interface(){};

    //metodos virtuais puros -> necessario implementar na classe derivada
    virtual void run(FileReader* rawFile) = 0;
    virtual FileReader* getFileReader(string filename) = 0;
    
};

#endif