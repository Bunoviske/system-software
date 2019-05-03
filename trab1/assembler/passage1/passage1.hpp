#ifndef PASSAGEM1
#define PASSAGEM1

#include "../interfaces/passage1interface.hpp"

class Passage1 : public Passage1Interface {

public:

    Passage1() { } //construtor
    ~Passage1() { }
    void run(FileReader* rawFile);
    FileReader* getFileReader(string filename);

private:


};

#endif