#include "utils.hpp"

//retorna o hexadecimal em formato char ascii
//extensao do sinal apenas para os numeros da secao de dados (sempre devem ter 32 bits)
string hex2ascii(uint32_t hexNumber, bool zeroExtension)
//é necessario usar essa funcao pois precisa transformar numero inteiro em hexa para ascii
//so é possivel fazer isso transformando cada dois hexadecimais para char e depois concatenando numa string
{
    stringstream ss;
    ss << hex << hexNumber;
    string hexString = ss.str();

    if (hexString.size() % 2 != 0) //se for impar, adiciona 0 no inicio do numero hexadecimal
        hexString = "0" + hexString;

    while (zeroExtension && hexString.size() < 8) //todo dado deve ter os 32 bits preenchidos (extensao de zero)
        hexString = "0" + hexString;

    //cout << hexString << endl;

    string result = "";
    for (size_t i = 0; i < hexString.size(); i += 2)
    {
        int hexTemp;
        stringstream str;
        str << hexString.substr(i, 2);
        str >> hex >> hexTemp;
        char hexChar = (char)hexTemp;
        result += string(1, hexChar);
    }
    return result;
}

uint32_t toLittleEndian(uint32_t hexNumber)
{
    return htobe32(hexNumber);
}

void openFile(string fileName, bool isReading, fstream &myFile)
{

    if (isReading)
        myFile.open(fileName, ios::in);
    else
        myFile.open(fileName, ios::out | ios::trunc);
}
void closeFile(fstream &myFile)
{
    myFile.close();
}

string readNextLine(fstream &myFile)
{
    string line;
    if (myFile.is_open())
    {
        if (getline(myFile, line))
            return line;
        else
            return "-1"; //eof
    }
}

vector<string> getTokensOfLine(string line)
{
    vector<string> tokens;
    string currentWord = "";

    for (size_t i = 0; i < line.size(); i++)
    {
        char charac = line[i];
        if (charac != ' ' && charac != '\n' && charac != '\t' && charac != ',')
        {
            if (charac == ';')
                //se for comentário, acaba a linha
                break;
            else
                currentWord += charac; //se for um caractere valido, adiciona ao token que esta sendo formado
        }
        else if (currentWord != "")
        {
            tokens.push_back(currentWord);
            currentWord = "";
        }
    }
    if (currentWord != "")
        tokens.push_back(currentWord);

    return tokens;
}

bool isDecimalNumber(string s)
{
    std::string::const_iterator it = s.begin();
    if (s.size() > 0 && s[0] == '-')
        if (s.size() == 1)
            return false; //se for so um traço, retorna false
        else
            it = s.begin() + 1;

    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool isHexadecimalNumber(string s)
{
    if (s.size() >= 3 && s[0] == '0' && s[1] == 'X')
    {
        std::string::const_iterator it = s.begin() + 2;
        while (it != s.end() && std::isxdigit(*it))
            ++it;
        return it == s.end();
    }
    return false;
}