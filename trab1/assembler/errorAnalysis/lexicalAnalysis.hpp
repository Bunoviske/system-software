#ifndef LEXICAL_ANALYSIS
#define LEXICAL_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "baseAnalyser.hpp"

class LexicalAnalyser : public BaseAnalyser {

public:

    LexicalAnalyser(){}; //construtor
    ~LexicalAnalyser(){};
    void run();

private:


};

#endif