#include <stdlib.h>
#include "InputStream.h"

void InputStream::openFile() {
    this->inputFile.open(this->inputFileName.c_str(), ios::binary);
    if (!this->inputFile.is_open()) {
        std::cout << "Could not open output file! " << this->inputFileName;
        exit(-1);
    }
}

InputStream::InputStream(std::string fileName) {
    this->inputFileName = fileName;
    this->openFile();
}

unsigned char InputStream::getNextChar() {
    unsigned char symbol=0;
    this->inputFile >> symbol;
    if (this->inputFile.eof()) {
        symbol = 0;
    }
    return symbol;

}

InputStream::~InputStream() {
    this->inputFile.close();
}