#ifndef FILE_READER
#define FILE_READER

#include "fileHandler.hpp"

class FileReader : public FileHandler {

public:

    FileReader(string fileName) : FileHandler(fileName,true){
        FileHandler(fileName,true);
    }; //construtor
    
    ~FileReader(){}; //destrutor
    
    string readNextLine();
};

#endif