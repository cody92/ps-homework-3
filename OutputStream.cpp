#include <fstream>
#include "OutputStream.h"

using namespace std;

void OutputStream::openFile() {
    this->outputFile.open(this->outputFileName.c_str(), ios::binary);
    if (!this->outputFile.is_open()) {
        std::cout << "Could not open output file! " << this->outputFileName;
        exit(-1);
    }
}

void OutputStream::OutputStream(std::string fileName) {
    this->outputFileName = fileName;
    this->openFile();
}

void OutputStream::writeToFile(std::string chunk) {
    std::cout << chunk << "\n";
}

void OutputStream::write() {
    std::string auxBuffer;
    auxBuffer = this->buffer.substr(0, OutputStream::MAX_BUFFER_SIZE - 1);
    this->writeToFile(auxBuffer);
    buffer = this->buffer.substr(MAX_BUFFER_SIZE - 1);
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

void OutputStream::~OutputStream() {
    this->write();
    this->outputFile.close();

}
