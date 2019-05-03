#include "passage2.hpp"

//override
FileReader* Passage2::getFileReader(string filename){
    return new FileReader(filename);
}

//override
FileWriter* Passage2::getFileWriter(string filename){
    return new FileWriter(filename);
}
    
//override
void Passage2::run(FileReader *rawFile, FileWriter *preprocFile){
  
    bool eof = false;
    while(!eof){

        string line = rawFile->readNextLine();
        if (line == "-1")
            eof = true;

        else{
            cout << line << endl;
            preprocFile->writeNextLine(line);
        }
    }    
    
}
