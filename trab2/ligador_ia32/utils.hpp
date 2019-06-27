#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <fstream>

/***********
 * Esses defines são vistos tanto pelo ligador quanto pelo elfioWriter
 ************/ 
#define TEXT_SEGMENT_ADDRESS 268435456 //0x10000000
#define DATA_SEGMENT_ADDRESS 285212672 //0x11000000

using namespace std;

string hex2ascii(uint64_t hexNumber);
uint32_t toLittleEndian(uint32_t hexNumber);

string readNextLine(fstream &myFile);
void openFile(string fileName, bool isReading, fstream &myFile);
void closeFile(fstream &myFile);
vector<string> getTokensOfLine(string line);
bool isHexadecimalNumber(string s);
bool isDecimalNumber(string s);

#endif

