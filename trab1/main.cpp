#include "montador/preproc/preprocService.hpp"
#include "montador/preproc/preproc.hpp"

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