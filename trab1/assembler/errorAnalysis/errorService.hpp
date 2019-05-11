#ifndef ERROR_SERVICE
#define ERROR_SERVICE

#include "../interfaces/errorAnalysisInterface.hpp"

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

class ErrorService : public ErrorInterface{

public:

    //construtor
    ErrorService() {

        lexical.setErrorListener(new LexicalListener());
        sintatic.setErrorListener(new SintaticListener());
        semantic.setErrorListener(new SemanticListener());
    };
    ~ErrorService(){};

    //override dos metodos virtuais
    void showErrorMessage(string message);
    int getErrorLine(string message);
    LexicalAnalyser getLexical(int line);
    SintaticAnalyser getSintatic(int line);
    SemanticAnalyser getSemantic(int line);

private:

    LexicalAnalyser lexical;
    SintaticAnalyser sintatic;
    SemanticAnalyser semantic;

};

#endif