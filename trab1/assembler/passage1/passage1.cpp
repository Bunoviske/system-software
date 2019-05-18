#include "passage1.hpp"

#define DEBUG 1 //liga varios prints para acompanhar a execucao
//override
FileReader* Passage1::getFileReader(string filename){
    #ifdef DEBUG
    cout << "___DEBUG - arquivo aberto: " << filename << endl;
    #endif
    return new FileReader(filename);
}

//ERROS CHECAGEM FALTA
//checar sintatxe de const e SPACE
//checar se const e space estao na secao correta
//override
void Passage1::run(FileReader *rawFile){

    bool eof = false;
    vector<string> words;
    int tokenType, positionCounter, lineCounter;
    string instruction, line, operation;

    positionCounter = 0;
    lineCounter = 0;


    while(!eof){
        #ifdef DEBUG
        cout << "___DEBUG - lendo linha passagem1: " << lineCounter << endl;
        #endif
        line =  rawFile->readNextLine();
        #ifdef DEBUG
        cout << "___DEBUG - linha passagem1: " << line <<  endl;
        #endif
        lineCounter++;

        if (line == "-1"){
            eof = true;
            tables.setSymbolTable("0lastSymbolAux", positionCounter);


        }
        else{
            //separa linha em rotulo, operacao e operando
            words = getTokensOfLine(line);
            tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);

            #ifdef DEBUG
            cout << "___DEBUG - token: " << words[0] << " tokenType: " << tokenType << endl;
            #endif

            /***** PRIMEIRO TOKEN EH LABEL *****/
            //se existir rotulo
            if(tokenType == DEF_LABEL){     //label
                words[0].pop_back(); //remove dois pontos da definicao do label
                //procura rotulo na tabela de simbolos
                #ifdef DEBUG
                cout << "___DEBUG - LABEL Encontrado == " << words[0] << endl;
                #endif
                if(errorService.getSemantic(lineCounter).isSymbolAlreadyDefined(words[0])){   //achou
                    //achou - erro, simbolo redefinido
                    /*ERRO*/
                    #ifdef DEBUG
                    cout << "___DEBUG - LABEL ja na tabela de simbolos, macro ou equ, ERRO - simbolo redefinido" << endl;
                    #endif
                }
                else{   //nao achou
                    //nao achou - insere rotulo e contador de posicao na tabela
                    tables.setSymbolTable(words[0], positionCounter);
                    #ifdef DEBUG
                    cout << "___DEBUG - LABEL nao encontrado na tabela, inserido" << endl;
                    #endif
                }
                words.erase(words.begin()); //remove label
                tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);    //tokenType do novo primeiro token da linha
                #ifdef DEBUG
                cout << "___DEBUG - Label removido, novo token: " << words[0] << endl;
                #endif
            }

            //CHECA SE EXISTE ALGO NA LINHA PARA CONTINUAR A PASSAGEM:
            //Caso em que nao ha - erro sintatico de definicao de label: label definido em uma linha em branco e label definido diretamente a seguir
            //checa se a operacao e valida
            if(errorService.getSemantic(lineCounter).isOperation(words[0])){
                operation = words[0];
                if(tokenType == INSTRUCTION){   //instrucao
                    //achou - contador de posicao = contador de posicao + tamanho da instrucao
                    positionCounter = positionCounter + tables.getInstructionSize(operation);
                    //analisa se a operacao possui os argumentos corretamente - sintatico - SEGUNDA PASSAGEM
                    // errorService.getSintatic(lineCounter).checkInstructionSintax(words);
                    #ifdef DEBUG
                    cout << "___DEBUG - Instrucao " << operation << " - Incrementa positionCounter com o tamanho da instrucao == " << positionCounter << endl;
                    #endif
                }
                else if(tokenType == DIRECTIVE){    //diretiva  -- execucao na segunda passagem apenas
                    //achou - executa diretiva / contador de posicao = valor retornado pela subrotina da diretiva
                    //positionCounter = diretiva(positionCounter, operation);
                    if(operation == "SPACE"){    //diretiva SPACE
                        if(errorService.getSemantic(lineCounter).isDirectiveInCorrectSection(operation)){
                            if(errorService.getSintatic(lineCounter).checkDirectiveSintax(words)){
                                positionCounter = positionCounter + checkSpaceSize(words);
                            }
                        }
                        #ifdef DEBUG
                        cout << "___DEBUG - Diretiva SPACE - Incrementa positionCounter com o tamanho do space == " << positionCounter << endl;
                        #endif
                    }

                    if(operation == "SECTION"){
                        errorService.getSintatic(lineCounter).checkSectionSintax(words);
                        // if(errorService.getSintatic(lineCounter).checkSectionSintax(words)){
                        //     errorService.getSemantic(lineCounter).setSectionLine(words[1]);
                        // }
                        #ifdef DEBUG
                        cout << "___DEBUG - Diretiva SECTION - Seta linhas" << endl;
                        #endif
                    }

                    if(operation == "CONST"){
                        if(errorService.getSemantic(lineCounter).isDirectiveInCorrectSection(operation)){
                            if(errorService.getSintatic(lineCounter).checkDirectiveSintax(words)){
                                positionCounter = positionCounter + 1;
                            }
                        }
                        #ifdef DEBUG
                        cout << "___DEBUG - Diretiva Const - Incrementa positionCounter == " << positionCounter << endl;
                        #endif
                    }

                }
            }


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
                #ifdef DEBUG
                cout << "___DEBUG - token: " << currentWord << endl;
                #endif
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


// int Passage1::checkSpaceSize(vector<string> words){
//     if(words.size() == 2){   //se a diretiva SPACE tiver argumento, pula o tamanho reservado
//         return stoi(words[2]);
//     }
//     return 1;       //se nao tiver argumento, reserva apenas 1 byte
// }
