#ifndef ELFIO_WRITER
#define ELFIO_WRITER

#include "utils.hpp"
#include "elfio/elfio.hpp"
#include "elfio/elfio_dump.hpp"

using namespace ELFIO;

//basePassage é usado pelo preprocessamento
class ElfioWriter
{

public:
    ElfioWriter()
    {
        writer.create(ELFCLASS32, ELFDATA2LSB);
        writer.set_os_abi(ELFOSABI_LINUX);
        writer.set_type(ET_EXEC);
        writer.set_machine(EM_386);

    };                //construtor
    ~ElfioWriter(){}; //destrutor

    void createTextSegment(string text);
    void createDataSegment(string data);
    void createExecutable(string filename);

private:

    elfio writer; 

};

#endif
