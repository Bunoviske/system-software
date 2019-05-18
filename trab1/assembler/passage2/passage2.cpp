#include "passage2.hpp"

#define DEBUG 1   //prints para acompanhar a execucao

//override
FileReader* Passage2::getFileReader(string filename){
    return new FileReader(filename);
}

//override
FileWriter* Passage2::getFileWriter(string filename){
    return new FileWriter(filename);
}

//override
void Passage2::run(FileReader *rawFile, FileWriter *preprocFile){
    vector<string> words;
    int tokenType, lineCounter, positionCounter;
    string line, procLine;
    bool eof = false;


    lineCounter = 0;
    positionCounter = 0;

    while(!eof){

        line = rawFile->readNextLine();
        if (line == "-1")
            eof = true;

        else{
            words = getTokensOfLine(line);
            tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);

            #ifdef DEBUG
            cout << "___DEBUG - token: " << words[0] << " tokenType: " << tokenType << endl;
            #endif

            if(tokenType == DEF_LABEL){ //ignora labels
                words.erase(words.begin());
                tokenType = errorService.getLexical(lineCounter).getTokenType(words[0]);
            }

            if(tokenType == INSTRUCTION){
                //checa sintaxe da Instrucao
                //checa se o label utilizado, caso utilizado, exista
                //se tudo ok, contador de posicao = contador de posicao + tamanho da Instrucao
                //gera codigo objeto conforme formato da instrucao
            }

            if(tokenType == DIRECTIVE){
                //checa sintaxe da diretiva -- ja checado na primeira passagem
                //executa diretiva

            }
            if(tokenType == LABEL){ //label, procura na tabela de simbolos
                //erro, sintaxe da instrucao = instrucao argumento
                // if(errorService.getSemantic(lineCounter).isSymbolDefined(words[0])){
                //     positionCounter =
                // }

            }


            lineCounter++;

            preprocFile->writeNextLine(procLine);
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
