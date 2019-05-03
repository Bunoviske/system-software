#ifndef SINTATIC_ANALYSIS
#define SINTATIC_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"

class SintaticAnalyser : public BaseAnalyser{

public:

    SintaticAnalyser(){}; //construtor
    ~SintaticAnalyser(){};
    void run();

private:
    

};

#endif