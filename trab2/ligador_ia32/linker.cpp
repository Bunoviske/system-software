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
    // cout << "Write Text Section" << endl;
    if (tokens[0] == "GLOBAL" || tokens[0] == "_start:") //pula quando eh a sinalizacao de _start
        return;

    if (isInstructionInTable(tokens[0])) //text += opcode
        text += getInstructionOpcode(tokens, nullptr);
    else
        cout << "Instrucao inexistente" << endl;

    if (tokens[0] == "INT")
        return; //argumento da instrucao INT nao deve ser processado, pois seu numero ja esta incluido no opcode

    getArgumentsBinaryCode(text, data, tokens, isJumpInstruction(tokens[0]));

    // text = hex2ascii(0xB804000000) + hex2ascii(0xBB01000000) + hex2ascii(0xB900000011) +
    //        hex2ascii(0xBA0E000000) + hex2ascii(0xCD80) + hex2ascii(0xB801000000) + hex2ascii(0xCD80);
    cout << text << endl;
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
        {
            vector<string> tokens = getTokensOfLine(line);
            if (tokens.size() > 0)
                searchSymbol(data, tokens);
        }
    }
    cout << "Tabela de simbolos na secao de texto:" << endl;
    for (std::map<string, uint32_t>::iterator it = textSymbolsTable.begin(); it != textSymbolsTable.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    cout << "Tabela de simbolos na secao de dados:" << endl;
    for (std::map<string, uint32_t>::iterator it = dataSymbolsTable.begin(); it != dataSymbolsTable.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';

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
            vector<string> tokens = getTokensOfLine(line);
            if (tokens.size() > 0)
                writeBinaryInstructions(text, data, tokens);
        }
    }
    closeFile(myFile);
}
void Linker::getArgumentsBinaryCode(string &text, string &data, vector<string> tokens, bool isJump)
{
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (isSymbolInTable(tokens[i])) //text += address do simbolo (displacement ou offset)
        {
            //cout << tokens[i] << endl;
            uint32_t val = DATA_SEGMENT_ADDRESS + (isJump ? textSymbolsTable[tokens[i]] : dataSymbolsTable[tokens[i]]);
            text += hex2ascii(toLittleEndian(val), true);
            // cout << hex << val << endl;
        }
        else if (tokens[i][0] == '[') //tem que acessar o dado armazenado
        {
            //retira [ ]
            tokens[i].pop_back();
            tokens[i].erase(tokens[i].begin());
            if (isSymbolInTable(tokens[i]))
            {
                int index = dataSymbolsTable[tokens[i]]; //acessa secao de dados e pega valor da memoria
                //cout << data.substr(index, 8) << endl;
                text += data.substr(index, 8); //8 digitos em hexadecimal representam 32 bits
            }
            else if (tokens[i].find('+') != string::npos)
            {
                string label = getLabelName(tokens[i]);
                if (!isSymbolInTable(label))
                    cout << "Simbolo nao definido" << endl;

                int displacement = getAddressDisplacement(tokens[i]);
                int index = dataSymbolsTable[label] + displacement; //acessa secao de dados e pega valor da memoria
                text += data.substr(index, 8);                      //8 digitos em hexadecimal representam 32 bits
                // cout << getAddressDisplacement(tokens[i]) << endl;
                // cout << getLabelName(tokens[i]) << endl;
                cout << hex << data.substr(index, 8) << ' ' << tokens[i] << endl;
            }
            else
                cout << "Registrador dentro de []" << endl;
        }
        else if (tokens[i].find('+') != string::npos) //displacement do endereco de memoria
        {

            string label = getLabelName(tokens[i]);
            if (!isSymbolInTable(label))
                cout << "Simbolo nao definido" << endl;

            int displacement = getAddressDisplacement(tokens[i]);
            uint32_t val = DATA_SEGMENT_ADDRESS + dataSymbolsTable[label] + displacement;
            text += hex2ascii(toLittleEndian(val), true);
            // cout << hex << val << ' ' << tokens[i] << endl;
        }
        else if (isDecimalNumber(tokens[i])) //imediato
        {
            uint32_t val = stoi(tokens[i]);
            text += hex2ascii(toLittleEndian(val), true);
        }
        else if (isHexadecimalNumber(tokens[i])) //imediato
        {
            uint32_t val = stoi(tokens[i], nullptr, 16);
            text += hex2ascii(toLittleEndian(val), true);
        }
    }
}
string Linker::getInstructionOpcode(vector<string> tokens, int *nextTokenToProcess)
{
    string completeInstruction = tokens[0];
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (tokens[i] == "DWORD" || tokens[i] == "WORD" || tokens[i] == "BYTE")
            continue;

        //se nao tiver na tabela e nao for imediato, adiciona a instrucao completa para verificar opcode.
        //TODO - Nao suporta displacement e nem acesso com [] !
        if (isSymbolInTable(tokens[i]) || isDecimalNumber(tokens[i]) || isHexadecimalNumber(tokens[i]))
        {
            if (tokens[i] == "0X80" && tokens[0] == "INT")
                //caso especial em que INT 0X80 ja é totalmente representado no opcode
                completeInstruction += " " + tokens[i];
            continue;
        }

        completeInstruction += " " + tokens[i];

        // if (tokens[i][0] == '[')
        // {
        //     //retira []
        //     tokens[i].pop_back();
        //     tokens[i].erase(tokens[i].begin());
        //     if (tokens[i].find('+') != string::npos)
        //     {
        //         string operand = getLabelName(tokens[i]);
        //         completeInstruction += isSymbolInTable(operand) ? "" : operand;
        //     }
        //     else
        //         completeInstruction += isSymbolInTable(tokens[i]) ? "" : tokens[i];
        // }
    }
    cout << completeInstruction << endl;
    return opcodesTable[completeInstruction];
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
    if (tokens[0] == "SECTION" && tokens[1] == ".text")
    {
        currentSection = TEXT_SECTION;
        return true;
    }
    else if (tokens[0] == "SECTION" && tokens[1] == ".data")
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
    if (tokens[0] == "_start:") //sempre deve ignorar o start (nao deve ser add na tabela de simbolos da secao de texto)
        return;

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
                    tokens[i].pop_back();           //tira o dois pontos ':'
                    dataSymbolsTable[tokens[i]] = dataSymbolAddress;
                    dataSymbolAddress += increment;
                    continue;
                }
            }
            //se chegar aqui é pq é um simbolo da secao de texto
            tokens[i].pop_back(); //tira o dois pontos ':'
            textSymbolsTable[tokens[i]] = textSymbolAddress;
            textSymbolAddress += getInstructionIncrementSize(tokens[1]); //instrucao esta na segunda posicao
        }
    }
}

uint32_t Linker::getInstructionIncrementSize(string instruction)
{
    // TODO
}

bool Linker::isSymbolInTable(string symbol)
{
    return !(textSymbolsTable.find(symbol) == textSymbolsTable.end()) |
           !(dataSymbolsTable.find(symbol) == dataSymbolsTable.end());
}

bool Linker::isInstructionInTable(string instruction)
{
    return !(instructionsTable.find(instruction) == instructionsTable.end());
}

bool Linker::isJumpInstruction(string instruction)
{
    if (instruction == "JMP" || instruction == "JL" || instruction == "JG" ||
        instruction == "JE" || instruction == "JNE" || instruction == "JB" ||
        instruction == "JA" || instruction == "JBE" || instruction == "JGE")

        return true;
    
    return false;
}
