#include "OutputStream.h"
#include <bits/stl_algo.h>
#include <bitset>

void OutputStream::openFile() {
    this->outputFile.open(this->outputFileName.c_str(), ios::binary);
    if (!this->outputFile.is_open()) {
        std::cout << "Could not open output file! " << this->outputFileName;
        exit(-1);
    }
    this->outputFile.unsetf(ios_base::skipws);
}

OutputStream::OutputStream(std::string fileName, bool mode) {
    this->outputFileName = fileName;
    this->mode = mode;
    this->openFile();
}

void OutputStream::writeToFile(std::string chunk) {

    if (chunk.size() < 8) {
        int size = chunk.size();
        while (size < 8) {
            chunk = chunk + "0";
            size++;
        }
    }
    std::bitset<8> temp(chunk);
    //std::cout << (int) temp.to_ulong() << " ";
    this->outputFile << (unsigned char) temp.to_ulong();
}

void OutputStream::write() {
    std::string auxBuffer;
    if (this->buffer.size() > OutputStream::MAX_BUFFER_SIZE) {
        auxBuffer = this->buffer.substr(0, OutputStream::MAX_BUFFER_SIZE);
        buffer = this->buffer.substr(MAX_BUFFER_SIZE);
    } else {
        auxBuffer = this->buffer;
    }

    this->writeToFile(auxBuffer);

    if (this->buffer.size() > OutputStream::MAX_BUFFER_SIZE) {
        this->write();
    }
}


void OutputStream::addData(std::string string) {
    this->buffer += string;
    if (buffer.size() > OutputStream::MAX_BUFFER_SIZE) {
        this->write();
    }
}

OutputStream::~OutputStream() {
    if (this->mode) {
        this->write();
    }
    this->outputFile.close();

}


void OutputStream::display(unsigned char symbol) {
    this->outputFile << symbol;
}
