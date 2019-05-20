#include "fileWriter.hpp"

void FileWriter::writeNextLine(string line)
{
    if (myFile.is_open())
    {
        myFile << line << endl;
    }
}

void FileWriter::writeNextOperation(string line)
{
    if (myFile.is_open())
    {
        myFile << line << flush;
    }
}