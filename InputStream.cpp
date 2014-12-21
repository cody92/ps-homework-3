#include <fstream>
#include "InputStream.h"

void InputStream::openFile() {
    this->inputFile.open(this->inputFileName.c_str(), ios::binary);
    if (!this->inputFile.is_open()) {
        std::cout << "Could not open output file! " << this->inputFileName;
        exit(-1);
    }
}

void InputStream::InputStream(std::string fileName) {
    this->inputFileName = fileName;
    this->openFile();
}

unsigned char InputStream::getNextChar() {
    unsigned char symbol;
    if (!this->inputFile.eof()) {
        this->inputFile >> symbol;
    } else {
        symbol = 0;
    }
    return symbol;

}

void InputStream::~InputStream() {
    this->inputFile.close();
}