#pragma once

#include "Huffman.h"
#include "InputStream.h"
#include "OutputStream.h"

class HuffmanCompress : public Huffman {
    InputStream *inputHandler;
    OutputStream *outputHandler;
public:
    HuffmanCompress(InputStream *, OutputStream *);

    ~HuffmanCompress();

    void process();

    unsigned char getNextChar();

    void addData(std::string);

    std::string getCode(GraphTree *);

    std::string getSymbolString(unsigned char);

    std::string getSymbolBits(unsigned char);
    void addEOS();


};