#ifndef PASSAGE1_SERVICE
#define PASSAGE1_SERVICE

#include "../interfaces/passage1interface.hpp"

class Passage1Service {

public:

    Passage1Interface *passage;

    //construtor
    Passage1Service(Passage1Interface *passage1) { 
        this->passage = passage1;
    };
    
    ~Passage1Service(){};

    void runService(string readFile);

private:


};

#endif