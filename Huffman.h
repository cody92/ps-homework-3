#pragma once

#include <vector>
#include <map>
#include <iostream>

using namespace std;
struct GraphTree {
    unsigned char symbol;
    unsigned char symbolExtended;
    unsigned long long int nrOfApp;
    GraphTree *parent;
    GraphTree *left;
    GraphTree *right;
    unsigned long long int index;
};

typedef std::vector<GraphTree *> GraphIndex;

typedef std::map<int, GraphTree *> MappedSymbols;
struct GraphSearch {
    bool type;
    GraphTree *reference;
};

class Huffman {
private:
    GraphTree *rootNode;
    const int ESC_VALUE = 257;
    const int EOS_VALUE = 256;
    MappedSymbols symbolMap;
    GraphIndex indexedGraph;
public:
    Huffman();

    ~Huffman();

    void initGraph();

    GraphSearch findSymbol(unsigned char);

    void updateSymbol(GraphSearch);

    void addSymbol(GraphSearch, unsigned char);

    void balanceGraph(GraphTree *);

    void updateMap();

    void encodeSymbol(unsigned char);

    void displayIndexedGraph();

    void displayGraph(GraphTree *);

    MappedSymbols getSymbolMap() {
        return symbolMap;
    }


    int const getESC_VALUE() const {
        return ESC_VALUE;
    }


    int const getEOS_VALUE() const {
        return EOS_VALUE;
    }


    GraphTree *getRootNode() const {
        return rootNode;
    }
};

