#include "translator/translate/translate.hpp"
#include "translator/translate/TranslateService.hpp"

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

    TranslateService preService(new TranslateProcessing());
    preService.runService(fileName, fileWithoutExtension + ".s");

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
