#ifndef PASSAGEM2
#define PASSAGEM2

#include "../interfaces/passage2interface.hpp"

class Passage2 : public Passage2Interface {

public:

    Passage2() { } //construtor
    ~Passage2() { }
    void run(FileReader* rawFile, FileWriter* preprocFile);
    FileReader* getFileReader(string filename);
    FileWriter* getFileWriter(string filename);

private:


};

#endif