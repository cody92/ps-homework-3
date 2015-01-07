#include <bitset>
#include "HuffmanDecompress.h"

HuffmanDecompress::HuffmanDecompress(InputStream *inputHandler, OutputStream *outputHandler) {
    this->inputHandler = inputHandler;
    this->outputHandler = outputHandler;
    Huffman();
    this->currentNode = this->getRootNode();
}

HuffmanDecompress::~HuffmanDecompress() {

}

unsigned char HuffmanDecompress::getNextChar() {
    return this->inputHandler->getNextChar();
}

void HuffmanDecompress::process() {
    unsigned char symbol;
    bool process = true;
    this->escFlag = false;
    this->symbolString = "";
    while ((symbol = this->getNextChar()) != 0 && process) {
        //std::cout << (int) symbol << " ";
        process = this->addData(symbol);
    }
}

bool HuffmanDecompress::addData(unsigned char symbol) {
    std::bitset<8> tempByte(symbol);
    return this->processByte(tempByte);
}

bool HuffmanDecompress::processByte(std::bitset<8> byte) {
    GraphTree *tempNode;
    std::string tempSymbol;
    unsigned long int tempCharSymbol;
    int symbolVal;
    tempNode = this->currentNode;
    for (int i = byte.size() - 1; i >= 0; --i) {
        //std::cout << byte[i];
        if (this->escFlag == true) {
            tempSymbol = byte.test(i) ? "1" : "0";
            this->symbolString = this->symbolString + tempSymbol;
            if (this->symbolString.size() == 8) {
                std::bitset<8> tempByteString(this->symbolString);
                tempCharSymbol = (unsigned long int) tempByteString.to_ullong();
                this->outputHandler->display((unsigned char) tempCharSymbol);
                this->escFlag = false;
                this->encodeSymbol((unsigned char) tempCharSymbol);
                tempNode = this->getRootNode();
//                this->displayIndexedGraph();
            }
        } else {
            if (byte.test(i)) {
                tempNode = tempNode->right;
            } else {
                tempNode = tempNode->left;
            }
            if (tempNode->symbol > 0) {
                symbolVal = tempNode->symbol + tempNode->symbolExtended;
                if (symbolVal == this->getESC_VALUE()) {
                    this->escFlag = true;
                    this->symbolString = "";
                } else if (symbolVal == this->getEOS_VALUE()) {
                    return false;
                } else {
                    this->outputHandler->display(tempNode->symbol);
                    this->encodeSymbol(tempNode->symbol);
//                    this->displayIndexedGraph();
                    tempNode = this->getRootNode();
                }
            }
        }
    }
    this->currentNode = tempNode;
    return true;
}
