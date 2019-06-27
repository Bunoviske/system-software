#include "elfioWriter.hpp"
#include "linker.hpp"

//#define DUMP

string getFileNameWithoutExtension(string filename);

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        cout << "Insira o nome do arquivo .s" << endl;
        return -1;
    }
    string fileName = argv[1];
    string fileWithoutExtension = getFileNameWithoutExtension(fileName);

    Linker linker;
    string text = "";
    string data = "";
    linker.run(text, data, fileName);

    ElfioWriter elfWriter;
    elfWriter.createTextSegment(text);
    elfWriter.createDataSegment(data);
    elfWriter.createExecutable(fileWithoutExtension);

    /**
         * PERMISSAO PARA EXECUTAR E EXECUCAO DO PROGRAMA COMPILADO
         **/
    cout << "\n************** Executando " + fileWithoutExtension << " **************" << endl
         << endl;
    string chmod = "chmod +x " + fileWithoutExtension;
    string execute = "./" + fileWithoutExtension;
    system(chmod.c_str());
    system(execute.c_str());
    cout << endl;

/**
         * DUMP ELF FILE
         **/
#ifdef DUMP
    cout << "\n************** Dumping " + fileWithoutExtension << " **************" << endl
         << endl;
    elfio reader;
    reader.load(fileWithoutExtension);
    dump::header(std::cout, reader);
    dump::section_headers(std::cout, reader);
    dump::segment_headers(std::cout, reader);
    dump::symbol_tables(std::cout, reader);
    dump::notes(std::cout, reader);
    dump::dynamic_tags(std::cout, reader);
    dump::section_datas(std::cout, reader);
    dump::segment_datas(std::cout, reader);
    cout << endl;
#endif

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
