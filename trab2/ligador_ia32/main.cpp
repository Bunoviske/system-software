#include "elfio/elfio.hpp"
#include "elfio/elfio_dump.hpp"
#include <iostream>

using namespace std;
using namespace ELFIO;

//#define DUMP

void relTable();
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

    elfio writer;
    writer.create(ELFCLASS32, ELFDATA2LSB);
    writer.set_os_abi(ELFOSABI_LINUX);
    writer.set_type(ET_EXEC);
    writer.set_machine(EM_386);

    /**
         * TEXT SECTION
         **/
    section *text_sec = writer.sections.add(".text");
    text_sec->set_type(SHT_PROGBITS);
    text_sec->set_flags(SHF_ALLOC | SHF_EXECINSTR);
    text_sec->set_addr_align(0x4);
    char text[] = {'\xB8', '\x04', '\x00', '\x00', '\x00', //mov eax, 4
                   '\xBB', '\x01', '\x00', '\x00', '\x00', // mov ebx, 1
                   '\xB9', '\x20', '\x00', '\x00', '\x08', // mov ecx, msg
                   '\xBA', '\x0E', '\x00', '\x00', '\x00', // mov edx, 14
                   '\xCD', '\x80',                         // int 0x80
                   '\xB8', '\x01', '\x00', '\x00', '\x00', // mov eax, 1
                   '\xCD', '\x80'};                        // int 0x80
    text_sec->set_data(text, sizeof(text));

    segment *text_seg = writer.segments.add();
    text_seg->set_type(PT_LOAD);
    text_seg->set_virtual_address(0x10000000);
    text_seg->set_physical_address(0x10000000);
    text_seg->set_flags(PF_X | PF_R);
    text_seg->set_align(0x1000);
    text_seg->add_section_index(text_sec->get_index(), text_sec->get_addr_align());

    // /**
    //  * STRING AND SYMBOL SECTION TABLE
    //  **/

    // // Create string table section
    // section *str_sec = writer.sections.add(".strtab");
    // str_sec->set_type(SHT_STRTAB);
    // string_section_accessor stra(str_sec);

    // Elf32_Word str_index = stra.add_string("msg");

    // // Create symbol table section
    // section *sym_sec = writer.sections.add(".symtab");
    // sym_sec->set_type(SHT_SYMTAB);
    // sym_sec->set_info(2);
    // sym_sec->set_addr_align(0x4);
    // sym_sec->set_entry_size(writer.get_default_entry_size(SHT_SYMTAB));
    // sym_sec->set_link(str_sec->get_index());
    // symbol_section_accessor syma(writer, sym_sec);

    // // Add symbol entry (msg has offset == 29)
    // Elf_Word sym_to_adjust = syma.add_symbol(str_index, 29, 0, STB_GLOBAL,
    //                                          STT_OBJECT, 0,
    //                                          text_sec->get_index());
    // // Another way to add symbol
    // syma.add_symbol(stra, "_start", 0x00000000, 0, STB_WEAK, STT_FUNC, 0,
    //                 text_sec->get_index());

    /**
         * DATA SECTION
         **/
    section *data_sec = writer.sections.add(".data");
    data_sec->set_type(SHT_PROGBITS);
    data_sec->set_flags(SHF_ALLOC | SHF_WRITE);
    data_sec->set_addr_align(0x4);

    char data[] = "Hello World\n";
    data_sec->set_data(data, sizeof(data));

    uint32_t val32 = 0x08000020;
    cout << hex << htobe32(val32) << endl;

    segment *data_seg = writer.segments.add();
    data_seg->set_type(PT_LOAD);
    data_seg->set_virtual_address(0x08000020);
    data_seg->set_physical_address(0x08000020);
    data_seg->set_flags(PF_W | PF_R);
    data_seg->set_align(0x1000);
    data_seg->add_section_index(data_sec->get_index(), data_sec->get_addr_align());

    writer.set_entry(0x10000000);
    writer.save(fileWithoutExtension);

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

    //relTable();

    return 1;
}

void relTable()
{
    elfio writer;

    // You can't proceed before this function call!
    writer.create(ELFCLASS32, ELFDATA2LSB);

    writer.set_os_abi(ELFOSABI_LINUX);
    writer.set_type(ET_EXEC | ET_REL);
    writer.set_machine(EM_386);

    // This is our code
    char text[] = {'\xB8', '\x04', '\x00', '\x00', '\x00', // mov eax, 4
                   '\xBB', '\x01', '\x00', '\x00', '\x00', // mov ebx, 1
                   '\xB9', '\x00', '\x00', '\x00', '\x00', // mov ecx, msg
                   '\xBA', '\x0E', '\x00', '\x00', '\x00', // mov edx, 14
                   '\xCD', '\x80',                         // int 0x80
                   '\xB8', '\x01', '\x00', '\x00', '\x00', // mov eax, 1
                   '\xCD', '\x80',                         // int 0x80
                   '\x48', '\x65', '\x6C', '\x6C', '\x6F', // msg: db   'Hello, World!', 10
                   '\x2C', '\x20', '\x57', '\x6F', '\x72',
                   '\x6C', '\x64', '\x21', '\x0A'};
    Elf64_Addr place_to_adjust = 11;

    // Create code section
    section *text_sec = writer.sections.add(".text");
    text_sec->set_type(SHT_PROGBITS);
    text_sec->set_flags(SHF_ALLOC | SHF_EXECINSTR);
    text_sec->set_addr_align(0x10);
    text_sec->set_data(text, sizeof(text));

    segment *text_seg = writer.segments.add();
    text_seg->set_type(PT_LOAD);
    // text_seg->set_virtual_address(0x08000000);
    // text_seg->set_physical_address(0x08000000);
    text_seg->set_flags(PF_X | PF_R);
    text_seg->set_align(0x1000);
    text_seg->add_section_index(text_sec->get_index(), text_sec->get_addr_align());

    // Create string table section
    section *str_sec = writer.sections.add(".strtab");
    str_sec->set_type(SHT_STRTAB);

    // Create string table writer
    string_section_accessor stra(str_sec);
    // Add label name
    Elf32_Word str_index = stra.add_string("msg");

    // Create symbol table section
    section *sym_sec = writer.sections.add(".symtab");
    sym_sec->set_type(SHT_SYMTAB);
    sym_sec->set_info(2);
    sym_sec->set_addr_align(0x4);
    sym_sec->set_entry_size(writer.get_default_entry_size(SHT_SYMTAB));
    sym_sec->set_link(str_sec->get_index());

    // Create symbol table writer
    symbol_section_accessor syma(writer, sym_sec);
    // Add symbol entry (msg has offset == 29)
    Elf_Word sym_to_adjust = syma.add_symbol(str_index, 29, 0, STB_GLOBAL,
                                             STT_OBJECT, 0,
                                             text_sec->get_index());
    // Another way to add symbol
    syma.add_symbol(stra, "_start", 0x00000000, 0, STB_WEAK, STT_FUNC, 0,
                    text_sec->get_index());

    // Create relocation table section
    section *rel_sec = writer.sections.add(".rel.text");
    rel_sec->set_type(SHT_REL);
    rel_sec->set_info(text_sec->get_index());
    rel_sec->set_addr_align(0x4);
    rel_sec->set_entry_size(writer.get_default_entry_size(SHT_REL));
    rel_sec->set_link(sym_sec->get_index());

    // Create relocation table writer
    relocation_section_accessor rela(writer, rel_sec);
    // Add relocation entry (adjust address at offset 11)
    rela.add_entry(place_to_adjust, sym_to_adjust,
                   (unsigned char)R_386_RELATIVE);

    // segment *data_seg = writer.segments.add();
    // data_seg->set_type(PT_LOAD);
    // data_seg->set_virtual_address(0x08000020);
    // data_seg->set_physical_address(0x08000020);
    // data_seg->set_flags(PF_W | PF_R);
    // data_seg->set_align(0x1000);
    // data_seg->add_section_index(data_sec->get_index(), data_sec->get_addr_align());

    writer.set_entry(0x00000000);
    writer.save("hello");
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
