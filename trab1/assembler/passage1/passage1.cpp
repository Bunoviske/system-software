#include "passage1.hpp"

//override
FileReader* Passage1::getFileReader(string filename){
    return new FileReader(filename);
}

//override
void Passage1::run(FileReader *rawFile){
  
    bool eof = false;
    while(!eof){

        string line = rawFile->readNextLine();
        if (line == "-1")
            eof = true;

        else{
            cout << line << endl;
        }
    }    
    
}
