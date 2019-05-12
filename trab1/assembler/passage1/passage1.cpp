#include "passage1.hpp"

//override
FileReader* Passage1::getFileReader(string filename){
    return new FileReader(filename);
}


//override
void Passage1::run(FileReader *rawFile){

    bool eof = false;
    vector<string> words;
    int tokenType, positionCounter, lineCounter;
    string instruction, line;

    positionCounter = 0;
    lineCounter = 0;


    while(!eof){

        line = rawFile->readNextLine();
        lineCounter++;

        if (line == "-1"){
            eof = true;

        }
        else{
            //separa linha em rotulo, operacao e operando
            words = getTokensOfLine(line);
            tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);

            /***** PRIMEIRO TOKEN EH LABEL *****/
            //se existir rotulo
            if(tokenType == DEF_LABEL){     //label
                //procura rotulo na tabela de simbolos
                if(errorService.getSemantic(lineNumber).isSymbolAlreadyDefined(words[0])){   //achou
                    //achou - erro, simbolo redefinido
                    /*ERRO*/
                }
                else{   //nao achou
                    //nao achou - insere rotulo e contador de posicao na tabela
                    tables.setSymbolTable(words[0], positionCounter);
                }
                words.erase(0); //remove label
                tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);    //tokenType do novo primeiro token da linha
            }

            //instrucao
            if(tokenType == INSTRUCTION){
                //achou - contador de posicao = contador de posicao + tamanho da instrucao
                positionCounter = positionCounter + tables.getInstructionSize(operation);
                //analisa se a operacao possui os argumentos corretamente - sintatico
                errorService.getSintatic(lineCounter).checkInstructionSintax(words);

            }
            else if(tokenType == DIRECTIVE){
                //achou - executa diretiva / contador de posicao = valor retornado pela subrotina da diretiva
                //positionCounter = diretiva(positionCounter, operation);

            }
            else if(tokenType == LABEL){
                //erro, label na posicao de uma operacao
            }


            // operation = words[0];
            //
            // //procura operacao na tabela de instrucoes
            // if(tables.isInstructionInTable(operation)){
            // }
            // else{
            //     //nao achou
            //     //procura operacao na tabela de diretivas
            //     if(tables.isDirectiveInTable(operation)){
            //     }
            //     else{
            //         //nao achou - erro, operacao nao identificada
            //         /*ERRO*/
            //     }
            //
            // }

        }

    }

}


vector<string> Passage1::getTokensOfLine(string line){ //temporario, possivelmente sera movida para a baseClass
    string currentWord;
    char tempChar;
    int i; /*size_t*/
    vector<string> words;


    for(i = 0; i < line.length(); i++){
        tempChar = line[i];

        if(tempChar == ' '){    //fim de um token
            if(currentWord != ""){
                words.push_back(currentWord);
                currentWord = "";
            }
        }
        else{
            currentWord = currentWord + tempChar;    //lendo um token
        }
    }
    if(currentWord != ""){
        words.push_back(currentWord);   //ultimo token da linha
    }

    return words;
}
