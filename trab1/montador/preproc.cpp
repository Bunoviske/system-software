#include "preproc.hpp"


void PreProcessing::run(string fileName){

    FileHandler fileHandler(fileName, true);
    
    bool eof = false;
    while(!eof){

        string line = fileHandler.readNextLine();
        if (line == "-1")
            eof = true;

        else{
            cout << line << endl;
        }
    }

}