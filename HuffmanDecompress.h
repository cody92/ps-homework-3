#pragma once

#include "Huffman.h"
#include "InputStream.h"
#include "OutputStream.h"

class HuffmanDecompress : public Huffman {
    InputStream *inputHandler;
    OutputStream *outputHandler;
    GraphTree *currentNode;
    bool escFlag;
    std::string symbolString;
public:
    HuffmanDecompress(InputStream *, OutputStream *);
    ~HuffmanDecompress();
    void process();
    unsigned char getNextChar();
    bool addData(unsigned char);
    bool processByte(std::bitset<8>);

};