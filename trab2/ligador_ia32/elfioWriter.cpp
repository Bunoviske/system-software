#include "elfioWriter.hpp"

void ElfioWriter::createTextSegment(string text)
{
    section *text_sec = writer.sections.add(".text");
    text_sec->set_type(SHT_PROGBITS);
    text_sec->set_flags(SHF_ALLOC | SHF_EXECINSTR);
    text_sec->set_addr_align(0x4);
    text_sec->set_data(text);

    segment *text_seg = writer.segments.add();
    text_seg->set_type(PT_LOAD);
    text_seg->set_virtual_address(TEXT_SEGMENT_ADDRESS);
    text_seg->set_physical_address(TEXT_SEGMENT_ADDRESS);
    text_seg->set_flags(PF_X | PF_R);
    text_seg->set_align(0x1000);
    text_seg->add_section_index(text_sec->get_index(), text_sec->get_addr_align());
}

void ElfioWriter::createDataSegment(string data)
{
    section *data_sec = writer.sections.add(".data");
    data_sec->set_type(SHT_PROGBITS);
    data_sec->set_flags(SHF_ALLOC | SHF_WRITE);
    data_sec->set_addr_align(0x4);
    data_sec->set_data(data);

    segment *data_seg = writer.segments.add();
    data_seg->set_type(PT_LOAD);
    data_seg->set_virtual_address(DATA_SEGMENT_ADDRESS);
    data_seg->set_physical_address(DATA_SEGMENT_ADDRESS);
    data_seg->set_flags(PF_W | PF_R);
    data_seg->set_align(0x1000);
    data_seg->add_section_index(data_sec->get_index(), data_sec->get_addr_align());
}

void ElfioWriter::createExecutable(string filename)
{
    writer.set_entry(TEXT_SEGMENT_ADDRESS);
    writer.save(filename);
}