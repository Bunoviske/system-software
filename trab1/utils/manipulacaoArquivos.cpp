#include "manipulacaoArquivos.hpp"


//construtor
FileHandler::FileHandler(string fileName, bool isReading)
{
    openFile(fileName, isReading);
}

string FileHandler::readNextLine()
{
    string line;
    if (myFile.is_open())
    {
        if (getline(myFile, line))
            return line;
        else
            return "-1"; //eof
    }
}
void FileHandler::writeNextLine(string line)
{

    if (myFile.is_open())
    {
        myFile << line << endl;
    }
}

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