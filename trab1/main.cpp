#include "montador/preproc.hpp"

int main( int argc, char** argv ){

    FileHandler fileHandler("teste.txt",false);
    
    bool eof = false;
    while(!eof){

        string line = fileHandler.readNextLine();
        if (line == "-1")
            eof = true;

        else{
            cout << line << endl;
        }
    }

    return 1;
}