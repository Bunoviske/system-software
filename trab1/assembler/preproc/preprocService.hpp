#ifndef PREPROC_SERVICE
#define PREPROC_SERVICE

#include "../interfaces/preprocInterface.hpp"

class PreProcessingService {

public:

    PreProcessingInterface *passage;

    //construtor
    PreProcessingService(PreProcessingInterface *preProcessing) { 
        this->passage = preProcessing;
    };
    
    ~PreProcessingService(){};

    void runService(string readFile, string writeFile);

private:


};

#endif