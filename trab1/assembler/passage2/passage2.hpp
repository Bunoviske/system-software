#ifndef PASSAGEM2
#define PASSAGEM2

#include "../interfaces/passage2interface.hpp"
#include "../errorAnalysis/errorService.hpp"
#include "../baseClass/baseClass.hpp"


class Passage2 : public Passage2Interface, private BaseClass {

public:

    Passage2() { } //construtor
    ~Passage2() { }
    void run(FileReader* rawFile, FileWriter* objFile);
    FileReader* getFileReader(string filename);
    FileWriter* getFileWriter(string filename);

private:

    ErrorService errorService;
    bool checkLabel(string s, int lineCounter);
    bool isSymbolOffset(vector<string> words, int labelIndex, int lineCounter);
    int checkSymbolOffset(vector<string> words, int labelIndex, int lineCounter);

};

#endif
