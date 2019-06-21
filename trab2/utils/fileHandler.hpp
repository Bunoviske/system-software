#ifndef FILE_HANDLER
#define FILE_HANDLER

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class FileHandler
{

public:
    FileHandler(string fileName, bool isReading)
    {
        openFile(fileName, isReading);
    }; //construtor

    ~FileHandler() { closeFile(); } //destrutor

protected:
    fstream myFile;

private:
    void openFile(string fileName, bool isReading);
    void closeFile();
};

#endif