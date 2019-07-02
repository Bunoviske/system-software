#include "linker.hpp"

void Linker::run(string &text, string &data, string filename)
{
    firstPassage(data, filename);  //set text and data symbols table
    secondPassage(text, filename); //write binary file
}

void Linker::writeTextSection(string &text, vector<string> tokens)
{
    if (tokens[0] == "GLOBAL" || tokens[0] == "_start:") //pula quando eh a sinalizacao de _start
        return;

    if (isInstructionInTable(tokens[0]))
        text += getInstructionOpcode(tokens);
    else
        cout << "Instrucao inexistente" << endl;

    if (tokens[0] == "INT" || tokens[0] == "CMP")
        return;
    //argumento da instrucao INT e CMP nao deve ser processado, pois seu numero ja esta incluido na
    //tabela de opcode. Esses argumentos nunca mudam para o assembly inventado

    getArgumentsBinaryCode(text, tokens, isJumpInstruction(tokens[0]));

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

void Linker::secondPassage(string &text, string filename)
{
    fstream myFile = fstream();
    openFile(filename, true, myFile);

    //variavel ira contar os enderecos de cada instrucao para que seja possivel calcular os jumps relativos.
    //o jump é relativo a instrucao seguinte, entao textSymbolAddress sempre vai atualizar antes de calcular os jumps
    textSymbolAddress = 0;

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
            {
                if (tokens[0] == "LERINTEIRO:")
                {
                    //a partir desse label, apenas copia-se o codigo maquina das funcoes feitas em assembly
                    text += getBinaryFunctions();
                    eof = true;
                }
                else
                    writeBinaryInstructions(text, tokens);
            }
        }
    }
    closeFile(myFile);
}

string Linker::getBinaryFunctions()
{
    fstream myFile = fstream();
    openFile("./functionsMachineCode.txt", true, myFile);

    bool eof = false;
    string allFunctions = "";
    int cont = 0;

    while (!eof)
    {
        string line = readNextLine(myFile);
        if (line == "-1")
            eof = true;
        else
        {
            vector<string> hexCodes = getTokensOfLine(line);
            if (hexCodes.size() > 0)
                cont++;

            for (size_t i = 0; i < hexCodes.size(); i++)
            {

                std::stringstream str;
                str << hexCodes[i];
                uint32_t val;
                str >> hex >> val;
                allFunctions += hex2ascii(val, false);
            }
        }
    }
    cout << cont << endl;
    closeFile(myFile);
    return allFunctions;
}
void Linker::getArgumentsBinaryCode(string &text, vector<string> tokens, bool isJump)
{
    //dentro desse for, so vai entrar nos ifs quando tiver que processar algum argumento do tipo imediato, label ou [label]
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (isSymbolInTable(tokens[i])) // label sem displacement
        {
            uint32_t val;
            //somente aqui pode acontecer um JUMP e um CALL (label sem [] e sem displacement)
            if (isJump)
            {
                if (tokens[0] == "CALL") //(endereco relativo de 32 bits) + offset da funcao LERINTEIRO = finalMainAddress
                {
                    //distancia da funcao ate LERINTEIRO + distancia atual do codigo ate LERINTEIRO
                    val = getAssemblyFunctionDistance(tokens[1]) + (finalMainAddress - textSymbolAddress);
                    text += hex2ascii(toLittleEndian(val), true);
                }
                else //algum dos JUMPS (endereco relativo de 8 bits)
                {
                    int relativeAddress = textSymbolsTable[tokens[1]] - textSymbolAddress; //label na posicao 1
                    cout << relativeAddress << endl;
                    if (relativeAddress < 0) //somar 256 se a diferenca entre os labels for negativa
                        relativeAddress += 256;
                    val = relativeAddress;
                    cout << relativeAddress << endl;
                    text += hex2ascii(val, false); //aqui nao é littleEndian
                }
            }
            else
            {
                val = DATA_SEGMENT_ADDRESS + dataSymbolsTable[tokens[i]];
                text += hex2ascii(toLittleEndian(val), true);
            }
        }
        else if (tokens[i][0] == '[')
        {
            //retira [ ]
            tokens[i].pop_back();
            tokens[i].erase(tokens[i].begin());
            if (isSymbolInTable(tokens[i])) // [label] sem displacement
            {
                uint32_t val = DATA_SEGMENT_ADDRESS + dataSymbolsTable[tokens[i]];
                text += hex2ascii(toLittleEndian(val), true);
            }
            else if (tokens[i].find('+') != string::npos) //displacement do endereco de memoria
            {
                string label = getLabelName(tokens[i]);
                if (!isSymbolInTable(label))
                    cout << "Simbolo nao definido" << endl;

                int displacement = getAddressDisplacement(tokens[i]);
                uint32_t val = DATA_SEGMENT_ADDRESS + dataSymbolsTable[tokens[i]] + displacement;
                text += hex2ascii(toLittleEndian(val), true);
            }
        }
        else if (tokens[i].find('+') != string::npos) //displacement do endereco de memoria
        {
            string label = getLabelName(tokens[i]);
            if (!isSymbolInTable(label))
                cout << "Simbolo nao definido" << endl;

            int displacement = getAddressDisplacement(tokens[i]);
            uint32_t val = DATA_SEGMENT_ADDRESS + dataSymbolsTable[label] + displacement;
            text += hex2ascii(toLittleEndian(val), true);
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
string Linker::getInstructionOpcode(vector<string> tokens)
{
    string completeInstruction = tokens[0];
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (tokens[i] == "DWORD" || tokens[i] == "WORD" || tokens[i] == "BYTE")
            continue;

        if (tokens[i][0] == '[') //caso o argumento tenha brackets, remove!
        {
            //retira []
            tokens[i].pop_back();
            tokens[i].erase(tokens[i].begin());
            completeInstruction += " []"; //adiciona brackets ao nome na posicao do argumento que acessa a memoria
        }

        string operand = tokens[i]; //caso tenha displacement, pega apenas nome do label
        if (tokens[i].find('+') != string::npos)
            operand = getLabelName(tokens[i]);

        //se nao tiver na tabela de simbolos e nao for imediato, adiciona a instrucao completa para verificar opcode.
        if (isSymbolInTable(operand) || isDecimalNumber(operand) || isHexadecimalNumber(operand))
        {
            if ((operand == "0X80" && tokens[0] == "INT") || (operand == "0" && tokens[0] == "CMP"))
                //caso especial em que INT 0X80 e CMP EAX 0 ja é totalmente representado no opcode.
                //Ele deve aparecer aqui pois na tabela esta representado como "INT 0X80" e "CMP EAX 0"
                completeInstruction += " " + operand;

            continue;
        }
        completeInstruction += " " + tokens[i];
    }
    if (tokens[0] == "PUSH")
        //push é a unica instrucao que deve ser checada na instructionsTable como completeInstruction (olhar tabela)
        textSymbolAddress += instructionsTable[completeInstruction];
    else
        textSymbolAddress += instructionsTable[tokens[0]];

    cout << completeInstruction << endl;
    return opcodesTable[completeInstruction];
}

void Linker::writeBinaryInstructions(string &text, vector<string> tokens)
{
    //IMPORTANTE: na primeira passagem ja é criado secao de dados, para que a segunda passagem saiba acessar
    //as posicoes de memoria dos labels

    if (setCurrentSection(tokens)) //se essa linha de codigo for definicao de secao, pula a escrita do codigo maquina
        return;

    if (currentSection == TEXT_SECTION)
    {
        if (tokens[0].back() == ':') //se primeiro argumento for label, deleta ele
        {
            tokens.erase(tokens.begin());
            if (tokens.size() == 0) //se so houver um label na linha (_start:), termina a funcao
                return;
        }
        writeTextSection(text, tokens);
    }
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
    setCurrentSection(tokens);

    if (tokens[0] == "_start:" || tokens[0] == "GLOBAL" || tokens[0] == "SECTION")
        //sempre deve ignorar esses simbolos (nao deve ser add na tabela de simbolos da secao de texto)
        return;

    if (tokens[0] == "LERINTEIRO:")
    {                                         // realiza a primeira passagem somente no assembly inventado
        finalMainAddress = textSymbolAddress; //LERINTEIRO é a primeira funcao depois do STOP
        cout << "Endereco final:" << endl;
        cout << finalMainAddress << endl;
    }

    if (currentSection != DATA_SECTION && finalMainAddress != 0) //nao processa as funcoes implementadas em assembly
        return;                                                  //textSymbolsTable so vai conter simbolos do assembly escrito pelo usuario (assembly inventado)

    uint32_t dataIncrement = 4;
    if (tokens[0].back() == ':')
    {
        if (tokens.size() > 1)
        {
            if (tokens[1] == "dd")
            { //checa se é um declare doubleword (dd)
                for (size_t j = 2; j < tokens.size(); j++)
                {
                    dataIncrement += 4; //incrementa o espaçamento para todos os tokens que tiverem depois de 'dd'
                }
                dataIncrement -= 4; //decrementa um espaçamento pois essa variavel ja começa com valor 4

                writeDataSection(data, tokens); //armazena na memoria os dados declarados 'dd'
                tokens[0].pop_back();           //tira o dois pontos ':'
                dataSymbolsTable[tokens[0]] = dataSymbolAddress;
                dataSymbolAddress += dataIncrement;
                return; //nao deve processar as linhas seguintes, entao termina a funcao
            }
        }
        //se chegar aqui é pq é um simbolo da secao de texto com instrucao na posicao 1
        tokens[0].pop_back(); //tira o dois pontos ':'

        if (isInTextSymbolTable(tokens[0]))
            cout << "Simbolo ja usado pelas funcoes implementadas em assembly de INPUT/OUTPUT" << endl;

        textSymbolsTable[tokens[0]] = textSymbolAddress;
        textSymbolAddress += getInstructionIncrementSize(tokens, 1); //instrucao na posicao 1
    }
    else
        //se chegar aqui é pq é uma instrucao da secao de texto
        textSymbolAddress += getInstructionIncrementSize(tokens, 0); //instrucao na posicao 0
}

uint32_t Linker::getInstructionIncrementSize(vector<string> tokens, int instructionPos)
{

    string instruction = tokens[instructionPos];
    //Porem, algumas instrucoes tem tamanhos diferentes dependendo da forma que sao chamadas (PUSH no assembly inventado)
    if (tokens[instructionPos] == "PUSH")
    { //PUSH so tem um argumento, por isso instructionPos+1

        if (tokens[instructionPos + 1] == "DWORD") //pula essa palavra
            instructionPos++;

        if (tokens[instructionPos + 1].find("EAX") != string::npos)
            instruction += " EAX"; //Na tabela de instrucoes esta "PUSH EAX"
        else if (tokens[instructionPos + 1].find('[') != string::npos)
            instruction += " []"; //Na tabela de instrucoes esta "PUSH []"
        //else -> nao altera a variavel instruction. //Na tabela de instrucoes esta "PUSH"
    }
    if (!isInstructionInTable(instruction))
        cout << instruction << " Instrucao nao esta na tabela" << endl;

    return instructionsTable[instruction];
}

bool Linker::isSymbolInTable(string symbol)
{
    return !(textSymbolsTable.find(symbol) == textSymbolsTable.end()) |
           !(dataSymbolsTable.find(symbol) == dataSymbolsTable.end());
}

bool Linker::isInTextSymbolTable(string symbol)
{
    return !(textSymbolsTable.find(symbol) == textSymbolsTable.end());
}

bool Linker::isInstructionInTable(string instruction)
{
    return !(instructionsTable.find(instruction) == instructionsTable.end());
}

bool Linker::isJumpInstruction(string instruction)
{
    if (instruction == "JMP" || instruction == "JL" || instruction == "JG" ||
        instruction == "JE" || instruction == "CALL")
        return true;

    return false;
}

uint32_t Linker::getAssemblyFunctionDistance(string function)
{
    if (function == "LERINTEIRO")
        return LER_INTEIRO_OFFSET;
    else if (function == "ESCREVERINTEIRO")
        return ESCREVER_INTEIRO_OFFSET;

    else if (function == "LERCHAR")
        return LER_CHAR_OFFSET;
    else if (function == "ESCREVERCHAR")
        return ESCREVER_CHAR_OFFSET;

    else if (function == "LERHEXA")
        return LER_HEXA_OFFSET;
    else if (function == "ESCREVERHEXA")
        return ESCREVER_HEXA_OFFSET;

    else if (function == "LERSTRING")
        return LER_STRING_OFFSET;
    else if (function == "ESCREVERSTRING")
        return ESCREVER_STRING_OFFSET;

    else
    {
        cout << "Call de funcao inexistente" << endl;
        return 0;
    }
}