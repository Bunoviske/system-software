#include "passage2.hpp"

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

    bool eof = false;
    while(!eof){

        string line = rawFile->readNextLine();      //recebe uma nova linha do arquivo
        if (line == "-1"){      //checa se o arquivo acabou
            eof = true;
        }
        else{
            cout << line << endl;
            preprocFile->writeNextLine(line);       //grava linha no arquivo objeto
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
