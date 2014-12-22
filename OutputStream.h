#pragma once

#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;
class OutputStream {
    std::string outputFileName;
    std::string buffer;
    std::ofstream outputFile;
    static const int MAX_BUFFER_SIZE = 8;

    void openFile();

    void writeToFile(std::string);

    void write();

public:
    OutputStream(std::string);

    ~OutputStream();

    void addData(std::string);
    void display(unsigned char);
};