#pragma once

#include <fstream>
#include <iostream>
using namespace std;
class InputStream {
    std::string inputFileName;
    std::ifstream inputFile;
    void openFile();

public:
    InputStream(std::string);
    unsigned char getNextChar();
    ~InputStream();
};