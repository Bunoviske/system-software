#ifndef SEMANTIC_ANALYSIS
#define SEMANTIC_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"

class SemanticAnalyser : public BaseAnalyser
{

public:
    SemanticAnalyser(){}; //construtor
    ~SemanticAnalyser(){};
    void run();

private:

};

#endif