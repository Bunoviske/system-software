#ifndef TRANSLATE_SERVICE
#define TRANSLATE_SERVICE

#include "../interfaces/ServiceInterface.hpp"

class TranslateService {

public:

    ServiceInterface *passage;

    //construtor
    TranslateService(ServiceInterface *passage) { 
        this->passage = passage;
    };
    
    ~TranslateService(){};

    void runService(string readFile, string writeFile);

private:


};

#endif