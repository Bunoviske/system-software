#ifndef BASE_ANALYSIS
#define BASE_ANALYSIS

#include "../interfaces/errorListener.hpp"
#include "../../utils/tablesService.hpp"

class LexicalListener : public ErrorListener{
    public:
        void onError(int line, string error){
            cout << "Erro lexico na linha: " << line << ". Erro: " << error << endl << endl;
        }
};

class SintaticListener : public ErrorListener{
    public:
        void onError(int line, string error){
            cout << "Erro sintatico na linha: " << line << ". Erro: " << error << endl << endl;
        }
};

class SemanticListener : public ErrorListener{
    public:
        void onError(int line, string error){
            cout << "Erro semantico na linha: " << line << ". Erro: " << error << endl << endl;
        }
};

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

    void throwError(string error){
        errorListener->onError(currentLine, error);
    }

};

#endif