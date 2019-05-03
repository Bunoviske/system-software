#ifndef PASSAGE2_SERVICE
#define PASSAGE2_SERVICE

#include "../interfaces/passage2interface.hpp"

class Passage2Service {

public:

    Passage2Interface *passage;

    //construtor
    Passage2Service(Passage2Interface *passage2) { 
        this->passage = passage2;
    };
    
    ~Passage2Service(){};

    void runService(string readFile, string writeFile);

private:


};

#endif