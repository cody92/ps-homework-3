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
    bool mode;

    void openFile();

    void writeToFile(std::string);

    void write();

public:
    OutputStream(std::string, bool);

    ~OutputStream();

    void addData(std::string);

    void display(unsigned char);
};