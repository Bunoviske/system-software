#include "passage1.hpp"

//override
FileReader* Passage1::getFileReader(string filename){
    return new FileReader(filename);
}


//override
void Passage1::run(FileReader *rawFile){

    bool eof = false;
    while(!eof){

        string line = rawFile->readNextLine();
        if (line == "-1")
            eof = true;

        else{
            cout << line << endl;
        }

        //separa linha em rotulo, operacao e operando

       //se existir rotulo
           //procura rotulo na tabela de simbolos
               //achou - erro, simbolo redefinido
               //nao achou - insere rotulo e contador de posicao na tabela

       //procura operacao na tabela de instrucoes
           //achou - contador de posicao = contador de posicao + tamanho da instrucao
           //nao achou
               //procura operacao na tabela de diretivas
               //achou - executa diretiva / contador de posicao = valor retornado pela subrotina da diretiva
               //nao achou - erro, operacao nao identificada
           //contador de linha++
    }



}
