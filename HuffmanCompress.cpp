#include "HuffmanCompress.h"
#include <bits/stl_algo.h>
#include <bitset>

HuffmanCompress::HuffmanCompress(InputStream *inputHandler, OutputStream *outputHandler) {
    this->inputHandler = inputHandler;
    this->outputHandler = outputHandler;
}

HuffmanCompress::~HuffmanCompress() {

}

unsigned char HuffmanCompress::getNextChar() {
    return this->inputHandler->getNextChar();
}

string HuffmanCompress::getSymbolString(unsigned char symbol) {
    std::bitset<8> temp(symbol);
    return temp.to_string();
}


std::string HuffmanCompress::getSymbolBits(unsigned char symbol) {
    GraphSearch result;
    std::string symbolCode, escCode;

    result = this->findSymbol(symbol);
    if (result.type == true) {
        symbolCode = this->getCode(result.reference);
        escCode = "";
    } else {
        escCode = this->getCode(this->getSymbolMap()[this->getESC_VALUE()]);
        symbolCode = this->getSymbolString(symbol);
    }
    return escCode + symbolCode;
}

void HuffmanCompress::process() {
    unsigned char symbol;
    std::string tempData, eosSymbol;
    GraphTree *tempEOS;
    while ((symbol = this->getNextChar()) != 0) {
        tempData = this->getSymbolBits(symbol);
        this->addData(tempData);
        this->encodeSymbol(symbol);
    }
    this->addEOS();
//    tempEOS = this->getSymbolMap()[this->getEOS_VALUE()];
//    eosSymbol = this->getCode();
//    this->addData(eosSymbol);
    //this->displayIndexedGraph();
}

void HuffmanCompress::addData(std::string string) {
    this->outputHandler->addData(string);
}

string HuffmanCompress::getCode(GraphTree *node) {
    string result;
    result = "";
    if (node->index % 2 == 0 && node->parent != NULL) {
        result = "0";
    } else if (node->parent != NULL) {
        result = "1";
    }

    if (node->parent != NULL) {
        return getCode(node->parent) + result;
    } else {
        return result;
    }
}


void HuffmanCompress::addEOS() {
    std::string eosSymbol;
    GraphTree *tempEOS;
    tempEOS = this->getSymbolMap()[this->getEOS_VALUE()];
    eosSymbol = this->getCode(tempEOS);
    this->addData(eosSymbol);
}
