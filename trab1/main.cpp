#include "montador/preproc.hpp"
#include "montador/passagem1.hpp"
#include "montador/passagem2.hpp"

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        cout << "Insira o nome do arquivo asm" << endl;
        return -1;
    }
    string fileName = argv[1];

    PreProcessing preProcessing;
    FirstPassage firstPassage;
    SecondPassage secondPassage;

    preProcessing.run(fileName);

    return 1;
}