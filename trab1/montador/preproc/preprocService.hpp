#ifndef PREPROC_SERVICE
#define PREPROC_SERVICE

#include "../preprocInterface.hpp"

class PreProcessingService {

public:

    PreProcessingInterface *preProcessing;

    //construtor
    PreProcessingService(PreProcessingInterface *preProcessing) { 
        this->preProcessing = preProcessing;
    };
    
    ~PreProcessingService(){};

    void runService(string readFile, string writeFile);

private:


};

#endif