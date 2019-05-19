#ifndef PASSAGEM1
#define PASSAGEM1

#include "../interfaces/passage1interface.hpp"
#include "../errorAnalysis/errorService.hpp"
#include "../baseClass/baseClass.hpp"
#include <locale.h>


class Passage1 : public Passage1Interface, private BaseClass{

public:

    Passage1() { } //construtor
    ~Passage1() { }
    void run(FileReader* rawFile);
    FileReader* getFileReader(string filename);

private:

    ErrorService errorService;
    int runOperation(vector<string> words, int lineCounter, int positionCounter);


};

#endif
