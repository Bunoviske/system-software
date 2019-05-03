#include "preproc.hpp"

//override
FileReader* PreProcessing::getFileReader(string filename){
    return new FileReader(filename);
}

//override
FileWriter* PreProcessing::getFileWriter(string filename){
    return new FileWriter(filename);
}
    
//override
void PreProcessing::run(FileReader *rawFile, FileWriter *preprocFile){
  
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

    errorService.getLexical(10).run();
    cout << tables.getDirectiveOperands("SPACE") << endl;
    
}

