#ifndef FILE_WRITER
#define FILE_WRITER

#include "fileHandler.hpp"

class FileWriter : public FileHandler {

public:

    FileWriter(string fileName) : FileHandler(fileName, false){}; //construtor
    ~FileWriter(); //destrutor

    void writeNextLine(string line);
   
};

#endif

