#ifndef ERROR_INTERFACE
#define ERROR_INTERFACE

#include <iostream>

#include "../errorAnalysis/lexicalAnalysis.hpp"
#include "../errorAnalysis/sintaticAnalysis.hpp"
#include "../errorAnalysis/semanticAnalysis.hpp"

using namespace std;

class ErrorInterface {

public:

    ErrorInterface(){};//construtor
    ~ErrorInterface(){};

    //metodos virtuais puros -> necessario implementar na classe derivada
    virtual void showErrorMessage(string message) = 0;
    virtual int getErrorLine(string message) = 0;
    virtual LexicalAnalyser getLexical(int line) = 0;
    virtual SintaticAnalyser getSintatic(int line) = 0;
    virtual SemanticAnalyser getSemantic(int line) = 0;

};

#endif