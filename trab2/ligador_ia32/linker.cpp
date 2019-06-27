#include "linker.hpp"

void Linker::run(string &text, string &data, string filename)
{
    firstPassage(filename); //set symbol table
    secondPassage(text, data, filename);
}

void Linker::writeTextSection(string &text, vector<string> tokens)
{
    // uint64_t val32 = 0xB900000000 + toLittleEndian(0x11000000);
    // cout << hex << val32 << endl;

    text = hex2ascii(0xB804000000) + hex2ascii(0xBB01000000) + hex2ascii(0xB900000011) +
           hex2ascii(0xBA0E000000) + hex2ascii(0xCD80) + hex2ascii(0xB801000000) + hex2ascii(0xCD80);
}
void Linker::writeDataSection(string &data, vector<string> tokens)
{
    //como so pode ser 'label: dd ...', o terceiro elemento em diante contem o dado a ser escrito na memoria
    for (size_t i = 2; i < tokens.size(); i++)
    {
        if (isHexadecimalNumber(tokens[i]))
        {
            data += hex2ascii(stoi(tokens[i], nullptr, 16));
        }
        else if (isDecimalNumber(tokens[i]))
        {
            data += hex2ascii(stoi(tokens[i]));
        }        
    }
    //data = "Hello World\n";
}

void Linker::firstPassage(string filename)
{
    fstream myFile = fstream();
    openFile(filename, true, myFile);

    bool eof = false;
    while (!eof)
    {
        string line = readNextLine(myFile);
        if (line == "-1")
            eof = true;
        else
            searchSymbol(getTokensOfLine(line));
    }

    // for (std::map<string, uint32_t>::iterator it = symbolsTable.begin(); it != symbolsTable.end(); ++it)
    //     std::cout << it->first << " => " << it->second << '\n';

    closeFile(myFile);
}

void Linker::secondPassage(string &text, string &data, string filename)
{
    fstream myFile = fstream();
    openFile(filename, true, myFile);

    bool eof = false;
    while (!eof)
    {
        string line = readNextLine(myFile);
        if (line == "-1")
            eof = true;
        else
        {
            writeBinaryFile(text, data, getTokensOfLine(line));
        }
    }
    closeFile(myFile);
}

void Linker::writeBinaryFile(string &text, string &data, vector<string> tokens)
{
    if (setCurrentSection(tokens)) //se essa linha de codigo for definicao de secao, pula a escrita do codigo maquina
        return;

    if (currentSection == TEXT_SECTION)
    {
        writeTextSection(text, tokens);
    }
    else if (currentSection == DATA_SECTION)
    {
        writeDataSection(data, tokens);
    }
}

bool Linker::setCurrentSection(vector<string> tokens)
{
    if (tokens[0] == "SECTION" && tokens[1] == ".TEXT")
    {
        currentSection = TEXT_SECTION;
        return true;
    }
    else if (tokens[0] == "SECTION" && tokens[1] == ".DATA")
    {
        currentSection = DATA_SECTION;
        return true;
    }
    return false;
}

void Linker::searchSymbol(vector<string> tokens)
{
    uint32_t increment = 4;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].back() == ':')
        {
            if (i != tokens.size() - 1)
            {
                if (tokens[i + 1] == "dd")
                { //checa se é um declare doubleword (dd)
                    for (size_t j = i + 2; j < tokens.size(); j++)
                    {
                        increment += 4; //incrementa o espaçamento para todos os tokens que tiverem depois de 'dd'
                    }
                    increment -= 4; //decrementa um espaçamento pois essa variavel ja começa com valor 4
                }
            }
            tokens[i].pop_back();
            symbolsTable[tokens[i]] = symbolAddress;
            symbolAddress += increment;
        }
    }
}

bool Linker::isSymbolInTable(string symbol)
{
    return !(symbolsTable.find(symbol) == symbolsTable.end());
}

bool Linker::isInstructionInTable(string instruction)
{
    return !(instructionsTable.find(instruction) == instructionsTable.end());
}
