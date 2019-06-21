#include "fileHandler.hpp"

void FileHandler::openFile(string fileName, bool isReading)
{

    if (isReading)
        myFile.open(fileName, ios::in);
    else
        myFile.open(fileName, ios::out | ios::trunc);
}
void FileHandler::closeFile()
{
    myFile.close();
}