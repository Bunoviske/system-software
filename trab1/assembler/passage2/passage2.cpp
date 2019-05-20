#include "passage2.hpp"

//#define DEBUG 1   //prints para acompanhar a execucao

//override
FileReader* Passage2::getFileReader(string filename){
    return new FileReader(filename);
}

//override
FileWriter* Passage2::getFileWriter(string filename){
    return new FileWriter(filename);
}

//override
void Passage2::run(FileReader *rawFile, FileWriter *objFile){
    vector<string> words;
    int tokenType, lineCounter, positionCounter, argNumber, i, arg1offset, arg2offset, arg2index;
    string line, procLine = "";
    bool eof = false;
    bool arg1label, arg2label, lineOk;



    lineCounter = 1;
    positionCounter = 0;

    #ifdef DEBUG
    cout << "___DEBUG_PASS2 - PASSAGEM2" << endl;
    #endif
    errorService.getSemantic(lineCounter).checkSectionOrder(); //checa se tem secao de texto e se a de dados vem depois

    while(!eof){

        line = rawFile->readNextLine();
        if (line == "-1")
            eof = true;

        else{
            words = getTokensOfLine(line);
            tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);
            lineOk = false;

            #ifdef DEBUG
            cout << "___DEBUG_PASS2 - token: " << words[0] << " tokenType: " << tokenType << endl;
            #endif

            if(tokenType == DEF_LABEL){ //ignora labels
                #ifdef DEBUG
                cout << "___DEBUG_PASS2 - LABEL Encontrado == " << words[0] << " IGNORADO" << endl;
                #endif
                words.erase(words.begin());
                tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);
            }

            //CHECA O MESMO QUE PASSAGEM 1 APOS DEFINICAO DE LABEL
            if(tokenType == INSTRUCTION){
                if(errorService.getSemantic(lineCounter).isInstructionInCorrectSection()){
                    #ifdef DEBUG
                    cout << "___DEBUG_PASS2 - Instrucao: " << words[0] << endl;
                    #endif

                    /*---------words[0] == instrucao;--------*/

                    //checa sintaxe da Instrucao. Nesse caso, checa apenas se o numero de argumentos é valido.
                    //posteriormente sera verificado se os operandos sao validos
                    if((errorService.getSintatic(lineCounter).checkInstructionSintax(words) && (errorService.getSemantic(lineCounter).checkInstructionSemantic(words)))){


                        argNumber = tables.getInstructionOperands(words[0]);
                        #ifdef DEBUG
                        cout << "___DEBUG_PASS2 - argnumber: " << argNumber << endl;
                        #endif

                        if(argNumber >= 1){
                            arg1label = checkLabel(words[1], lineCounter);  //primeiro argumento eh label
                            if(isSymbolOffset(words, 1, lineCounter)){
                                #ifdef DEBUG
                                cout << "___DEBUG_PASS2 - arg1 possui offset" << endl;
                                #endif
                                arg2index = 4;
                            }
                            else{
                                #ifdef DEBUG
                                cout << "___DEBUG_PASS2 - arg1 nao possui offset" << endl;
                                #endif
                                arg2index = 2;
                            }
                            if(words[0] == "COPY"){
                                #ifdef DEBUG
                                cout << "___DEBUG_PASS2 - copy - virgula removida do fim do arg1: " << words[arg2index-1] << endl;
                                #endif
                                words[arg2index-1].erase(words[arg2index-1].end()-1); //remove a virgula do final do primeiro operando do copy
                            }
                            if(argNumber == 2){
                                arg2label = checkLabel(words[arg2index], lineCounter);  //segundo argumento eh label
                            }
                        }


                        #ifdef DEBUG
                        cout << "___DEBUG_PASS2 - Numero de argumentos " << argNumber << endl;
                        cout << "___DEBUG_PASS2 - 1 label: " << arg1label << " 2 label: " << arg2label << endl;
                        #endif


                        procLine = to_string(tables.getInstructionCode(words[0]));
                        #ifdef DEBUG
                        cout << "___DEBUG_PASS2 - Code da instrucao " << tables.getInstructionCode(words[0]) << endl;
                        #endif


                        if(argNumber >= 1){ //1 ou 2 argumentos
                            if(arg1label){  //primeiro eh label
                                if(errorService.getSemantic(lineCounter).isSymbolDefined(words[1])){ //esta definido
                                    procLine = procLine + ' ' + to_string(tables.getSymbolAddress(words[1]) + checkSymbolOffset(words, 1, lineCounter));
                                    #ifdef DEBUG
                                    cout << "___DEBUG_PASS2 - Endereco do label: " << words[1] << " - " <<to_string(tables.getSymbolAddress(words[1]) + checkSymbolOffset(words, 1, lineCounter)) << endl;
                                    #endif
                                    lineOk = true;
                                }
                                else{ //nao definido - instrucao nao sera compilada
                                    lineOk = false;
                                }
                            }
                            else{
                                // lineOk = false;
                                // procLine = procLine + ' ' + words[1]; //se nao for label, eh imediato ??
                            }

                            if(argNumber == 2){ //dois argumentos
                                if(arg2label){ //segundo eh label
                                    if(errorService.getSemantic(lineCounter).isSymbolDefined(words[arg2index])){ //esta definido
                                        procLine = procLine + ' ' + to_string(tables.getSymbolAddress(words[arg2index]) + checkSymbolOffset(words, 2, lineCounter));
                                        lineOk = true;
                                    }
                                    else{   //nao definido - instrucao nao sera compilada
                                        lineOk = false;
                                    }
                                }
                                else{   //se nao for label, eh imediato ??
                                    // lineOk = false;
                                    // procLine = procLine + ' ' + words[2];
                                }
                            }
                        }
                        else if(argNumber == 0){
                            lineOk = true;
                        }
                        //else - redundante. caso sejam 0 argumentos, nada extra precisa ser feito

                        #ifdef DEBUG
                        cout << "___DEBUG_PASS2 - procLine: " << procLine << endl;
                        cout << "___DEBUG_PASS2 - lineOk: " << lineOk << endl;
                        #endif

                        if(lineOk){
                            positionCounter = positionCounter + tables.getInstructionSize(words[0]);
                            #ifdef DEBUG
                            cout << "___DEBUG_PASS2 - PositonCounter: " << positionCounter << endl;
                            #endif
                        }




                        //se tudo ok, contador de posicao = contador de posicao + tamanho da Instrucao
                        //gera codigo objeto conforme formato da instrucao
                        //CHECA SEMANTICA DA INSTRUCAO

                    }
                }
                else{
                    lineOk = false;
                }
            }

            if(tokenType == DIRECTIVE){ //todos os erros de diretiva ja foram checados na primeira passagem

                lineOk = true;
                //checa sintaxe da diretiva -- ja checado na primeira passagem
                //executa diretiva
                if(words[0] == "SPACE"){
                    #ifdef DEBUG
                    cout << "___DEBUG_PASS2 - SPACE: " << checkSpaceSize(words) << endl;
                    #endif
                    int spaceNum = checkSpaceSize(words);
                    positionCounter = positionCounter + spaceNum;
                    for(i=0;i<spaceNum;i++){
                        procLine = procLine + "00";
                        if (i < spaceNum-1) //nao add espaco no ultimo space
                            procLine += ' ';
                    }
                }
                if(words[0] == "CONST" && words.size() == 2)
                { //garante q ha pelo menos duas palavras. O erro é dado na passagem 1
                    #ifdef DEBUG
                    cout << "___DEBUG_PASS2 - CONST: " << words[1] << endl;
                    #endif
                    positionCounter = positionCounter + 1;
                    if(errorService.getLexical(lineCounter).isHexadecimalNumber(words[1])){ //nao gera erro
                        words[1].erase(words[1].begin(), words[1].begin()+2);
                        procLine = to_string(stoi(words[1], NULL, 16));
                    }
                    else{
                        procLine = words[1];
                    }
                }
                if(words[0] == "SECTION"){
                    lineOk = false;
                }

                #ifdef DEBUG
                cout << "___DEBUG_PASS2 - PositonCounter: " << positionCounter << endl;
                #endif




            }

            // if(tokenType == LABEL){ //label, procura na tabela de simbolos
            //     //erro, sintaxe da instrucao - label como instrucao
            //
            // }


            lineCounter++;

            if(lineOk){ //escreve no arquivo apenas se a linha estiver ok
                objFile->writeNextLine(procLine); //TODO - ESCREVER NO ARQUIVO E ADICIONAR UM ' ' NO FINAL DE PROCLINE
                #ifdef DEBUG
                cout << "___DEBUG_PASS2 - ProcLine " << procLine << endl;
                #endif
            }
            procLine = "";
        }



        //separa os elementos da linha, ignora rotulos
        //para cada operando que for um simbolo, procura na tabela de simbolos
            //nao achou - erro, simbolo indefinido
            //achou - contador de posicao = contador de posicao + tamanho da instrucao
                //se numero e tipo dos operandos esta correto
                    //gera codigo objeto conforme formato da instrucao
                //senao - erro, operacao nao identificada
            //nao achou - procura operacao na tabela de diretivas
                //achou - executa diretiva / contador de posicao = valor retornado pela subrotina da diretiva
                //nao achou - erro, operacao nao identificada
        //contador de linha++
    }

}

bool Passage2::checkLabel(string s, int lineCounter){
    if(s.back() == ','){
        s.erase(s.end()-1);
    }
    return errorService.getSintatic(lineCounter).checkInstructionOperandSintax(s);
}

bool Passage2::isSymbolOffset(vector<string> words, int labelIndex, int lineCounter){
    bool check = false;
    #ifdef DEBUG
    cout << "___DEBUG_PASS2 - isSymbolOffset chamado " << endl;
    #endif
    if(words[labelIndex] != words.back()){
        if(words[labelIndex+1] == "+"){
            #ifdef DEBUG
            cout << "___DEBUG_PASS2 - arg possui + "<< endl;
            #endif
            if(errorService.getSintatic(lineCounter).checkSymbolOffsetSintax(words, labelIndex)){
                #ifdef DEBUG
                cout << "___DEBUG_PASS2 - sintaxe + correta " << endl;
                #endif
                check = true;
            }
            else{
                #ifdef DEBUG
                cout << "___DEBUG_PASS2 - sintaxe + incorreta " << endl;
                #endif
                words.erase(words.begin()+labelIndex+1);    //erro na sintaxe == possui + mas nao possui numero = remove o +, trata como sem offset
                #ifdef DEBUG
                cout << "___DEBUG_PASS2 - removido + do argumento " << endl;
                #endif
            }
        }
        else{   /*DESNECESSARIO - APENAS PARA DEBUG - REMOVER DO FINAL------------------------------------------------------------*/
            #ifdef DEBUG
            cout << "___DEBUG_PASS2 - arg nao possui +" << endl;
            #endif
        }
    }
    else{   /*DESNECESSARIO - APENAS PARA DEBUG - REMOVER DO FINAL------------------------------------------------------------*/
        #ifdef DEBUG
        cout << "___DEBUG_PASS2 - arg1 eh o ultimo token " << endl;
        #endif
    }
    return check;
}

int Passage2::checkSymbolOffset(vector<string> words, int labelIndex, int lineCounter){
    int offset = 0;
    if(words[labelIndex] != words.back()){
        if(words[labelIndex+1] == "+" && words[labelIndex + 1] != words.back()){
            if(errorService.getSemantic(lineCounter).isSymbolOffsetCorrect(words[labelIndex], stoi(words[labelIndex+2]))){
                offset = stoi(words[labelIndex+2]);
                #ifdef DEBUG
                cout << "___DEBUG_PASS2 - Label com offset: " << offset << endl;
                #endif
            }
        }
        else{   /*DESNECESSARIO - APENAS PARA DEBUG - REMOVER DO FINAL------------------------------------------------------------*/
            #ifdef DEBUG
            cout << "___DEBUG_PASS2 - Label sem offset: " << offset << endl;
            #endif
        }
    }
    else{   /*DESNECESSARIO - APENAS PARA DEBUG - REMOVER DO FINAL------------------------------------------------------------*/
        #ifdef DEBUG
        cout << "___DEBUG_PASS2 - Label sem offset - ultimo token da linha" << offset << endl;
        #endif
    }
    return offset;
}
