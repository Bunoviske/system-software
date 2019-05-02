#include "fileReader.hpp"

string FileReader::readNextLine()
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
