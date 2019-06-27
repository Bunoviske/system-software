#include "linker.hpp"

void Linker::run(string &text, string &data, string filename)
{
    // uint32_t val = toLittleEndian(0x00000011);
    // cout << hex << val << endl;

    firstPassage(data, filename); //set symbol table
    secondPassage(text, data, filename);
}

void Linker::writeTextSection(string &text, string &data, vector<string> tokens)
{

    if (tokens[0] == "GLOBAL" || tokens[0] == "_start:") //pula quando eh a sinalizacao de start
        return;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (isInstructionInTable(tokens[i])) //text += opcode
        {
        }
        else if (isSymbolInTable(tokens[i])) //text += address do simbolo (displacement ou offset)
        {
            //cout << tokens[i] << endl;
            uint32_t val = DATA_SEGMENT_ADDRESS + symbolsTable[tokens[i]]; //numero ja tem os 32 bits extendidos pois DATA SEGMENT tem 32bits
            text += hex2ascii(toLittleEndian(val), false);
            // cout << hex << val << endl;
        }
        else if (tokens[i][0] == '[') //tem que acessar o dado armazenado
        {
            //retira [ ]
            tokens[i].pop_back();
            tokens[i].erase(tokens[i].begin());
            if (isSymbolInTable(tokens[i]))
            {
                int index = symbolsTable[tokens[i]]; //acessa secao de dados e pega valor da memoria
                //cout << data.substr(index, 8) << endl;
                text += data.substr(index, 8); //8 digitos em hexadecimal representam 32 bits
            }
            else if (tokens[i].find('+') != string::npos)
            {
                // cout << tokens[i] << endl;
                // cout << getAddressDisplacement(tokens[i]) << endl;
                // cout << getLabelName(tokens[i]) << endl;
            }
            else 
                cout << "Erro na montagem das instrucoes" << endl;
        }
        else if (tokens[i].find('+') != string::npos) //displacement do endereco de memoria
        {
            // cout << tokens[i] << endl;
            // cout << getAddressDisplacement(tokens[i]) << endl;
            // cout << getLabelName(tokens[i]) << endl;
        }
        else
        {
            cout << "Erro na montagem das instrucoes" << endl;
        }
    }

    // text = hex2ascii(0xB804000000) + hex2ascii(0xBB01000000) + hex2ascii(0xB900000011) +
    //        hex2ascii(0xBA0E000000) + hex2ascii(0xCD80) + hex2ascii(0xB801000000) + hex2ascii(0xCD80);
}
void Linker::writeDataSection(string &data, vector<string> tokens)
{
    //como so pode ser 'label: dd ...', o terceiro elemento em diante contem o dado a ser escrito na memoria
    for (size_t i = 2; i < tokens.size(); i++)
    {
        if (isHexadecimalNumber(tokens[i]))
        {
            data += hex2ascii(stoi(tokens[i], nullptr, 16), true);
        }
        else if (isDecimalNumber(tokens[i]))
        {
            data += hex2ascii(stoi(tokens[i]), true);
        }
    }
    //data = "Hello World\n";
}

void Linker::firstPassage(string &data, string filename)
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
            searchSymbol(data, getTokensOfLine(line));
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
            writeBinaryInstructions(text, data, getTokensOfLine(line));
        }
    }
    closeFile(myFile);
}

void Linker::writeBinaryInstructions(string &text, string &data, vector<string> tokens)
{
    //IMPORTANTE: na primeira passagem ja é criado secao de dados, para que a segunda passagem saiba acessar
    //as posicoes de memoria dos labels por meio de [L1]

    if (setCurrentSection(tokens)) //se essa linha de codigo for definicao de secao, pula a escrita do codigo maquina
        return;

    if (currentSection == TEXT_SECTION)
    {
        writeTextSection(text, data, tokens);
    }
    // else if (currentSection == DATA_SECTION)
    // {
    //     writeDataSection(data, tokens);
    // }
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

int Linker::getAddressDisplacement(string token)
{
    int pos = token.find('+');
    string displacement = token.substr(pos + 1);
    return stoi(displacement);
}
string Linker::getLabelName(string token)
{
    int pos = token.find('+');
    string label = token.substr(0, pos);
    return label;
}

void Linker::searchSymbol(string &data, vector<string> tokens)
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

                    writeDataSection(data, tokens); //armazena na memoria os dados declarados 'dd'
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
