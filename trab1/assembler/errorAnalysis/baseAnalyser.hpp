#ifndef BASE_ANALYSIS
#define BASE_ANALYSIS

#include "../interfaces/errorListener.hpp"

class BaseAnalyser {

public:

    BaseAnalyser(){}; //construtor
    ~BaseAnalyser(){};

    void setErrorListener(ErrorListener *errorListener){
        this->errorListener = errorListener;
    }
    void setLineNumber(int line){
        this->currentLine = line;
    }

protected:

    int currentLine;
    ErrorListener *errorListener;

};

#endif