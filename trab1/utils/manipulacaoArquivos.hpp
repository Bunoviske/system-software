#ifndef MANIPULACAO_ARQUIVOS
#define MANIPULACAO_ARQUIVOS

#include <iostream>
#include <fstream>

using namespace std;


class FileHandler {

public:

    FileHandler(string fileName, bool isReading); //construtor
    ~FileHandler() { closeFile(); } //destrutor

    string readNextLine();
    void writeNextLine(string line);
    

private:

    fstream myFile;
    void openFile(string fileName, bool isReading);
    void closeFile();
};

#endif