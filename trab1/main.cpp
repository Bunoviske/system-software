#include "assembler/preproc/preprocService.hpp"
#include "assembler/preproc/preproc.hpp"

#include "assembler/passage1/passage1.hpp"
#include "assembler/passage1/passage1Service.hpp"

#include "assembler/passage2/passage2.hpp"
#include "assembler/passage2/passage2Service.hpp"

string getFileNameWithoutExtension(string filename);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Insira o nome do arquivo asm" << endl;
        return -1;
    }
    string fileName = argv[1];
    string fileWithoutExtension = getFileNameWithoutExtension(fileName);

    PreProcessingService preService(new PreProcessing());
    preService.runService(fileName, fileWithoutExtension + ".pre");

    // Passage1Service pass1Service(new Passage1());
    // pass1Service.runService(fileWithoutExtension + ".pre");

    // Passage2Service pass2Service(new Passage2());
    // pass2Service.runService(fileWithoutExtension + ".pre", fileWithoutExtension + ".obj");

    return 1;
}

string getFileNameWithoutExtension(string filename)
{
    string result = "";
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename[i] == '.')
            break;
        else
            result += filename[i];
    }
    return result;
}