#include "sintaticAnalysis.hpp"

//IMPLEMENTAR FUNCOES!!!!
//TODO - TODA VEZ QUE CHAMAR LEXICO, TEM QUE CHAMAR A FUNCAO lexical.setCurrentLine();
bool SintaticAnalyser::checkInstructionSintax(vector<string> &tokens) { return true;}

bool SintaticAnalyser::checkDirectiveSintax(vector<string> tokens) {
    if(tokens[0] == "SPACE"){
        if(!checkSpaceSintax(tokens)){
            return false;
        }
    }
    else if(tokens[0] == "CONST"){
        if(!checkConstSintax(tokens)){
            return false;
        }
    }
    else{
        //outras diretivas
    }

}

bool SintaticAnalyser::checkLabelDefinitionSintax(vector<string> &tokens) {return true;}
bool SintaticAnalyser::checkMacroCallSintax(vector<string> &tokens) {return true;}


bool SintaticAnalyser::checkSectionSintax(vector<string> tokens){
    if (tokens.size() <= 1){
        throwError("A diretiva section requer um argumento");
        return false;
    }
    else if(tokens.size() > 2){
        throwError("A diretiva section so admite um argumento");
        return false;
    }
    if ((tokens[1] != "DATA")&&(tokens[1] != "TEXT")){
        throwError("Secao nao existente");
        return false;
    }
    return true;
}

bool SintaticAnalyser::checkSpaceSintax(vector<string> tokens){

    if (tokens.size() > 2){
        throwError("Diretiva SPACE admite apenas 0 ou 1 argumento");
        return false;
    }
    if(tokens.size() == 2){
        lexical.setLineNumber(this->currentLine);
        if(!lexical.getTokenType(tokens[1]) == NUMBER){
            throwError("Diretiva SPACE admite apenas argumentos numericos");
            return false;
        }
    }
    return true;
}

bool SintaticAnalyser::checkConstSintax(vector<string> tokens){

    if(tokens.size() == 1){
        throwError("Diretiva CONST precisa de um argumento");
        return false;
    }
    else if(tokens.size() > 2){
        throwError("Diretiva CONST admite apenas um argumento");
        return false;
    }
    else if(tokens.size() == 2){
        lexical.setLineNumber(this->currentLine);
        if(!lexical.getTokenType(tokens[1]) == NUMBER){
            throwError("Diretiva CONST admite apenas argumentos numericos");
            return false;
        }
    }
    return true;
}
