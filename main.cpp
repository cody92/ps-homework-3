#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <bits/stl_algo.h>
#include <bitset>
#include <fstream>
#include <chrono>

using namespace std;

typedef struct GraphTree {
    unsigned char symbol;
    unsigned char symbolExtended;
    unsigned long long int nrOfApp;
    GraphTree *parent;
    GraphTree *left;
    GraphTree *right;
    unsigned long long int index;
} ShannonTree;

typedef std::vector<GraphTree *> GraphIndex;

typedef std::map<int, std::pair<GraphTree *, std::string>> MappedSymbols;
typedef std::chrono::duration<int, std::milli> miliseconds_type;


typedef struct GraphSearch {
    std::string code;
    bool type;
    GraphTree *reference;
};

int ENCODE_BITS = 8;
int ESC_VALUE = 257;
int EOS_VALUE = 256;
MappedSymbols symbolMap;
GraphIndex indexedGraph;


auto
        cmp = [](std::pair<unsigned char, unsigned long int> const &a, std::pair<unsigned char, unsigned long int> const &b) {
    return a.second != b.second ? a.second > b.second : a.first < b.first;
};

GraphTree *initGraph() {

    GraphTree *parent, *left, *right;
    parent = new GraphTree;
    left = new GraphTree;
    right = new GraphTree;
    parent->symbol = 0;
    parent->symbolExtended = 0;
    parent->nrOfApp = 2;


    parent->parent = NULL;
    parent->left = left;
    parent->right = right;
    parent->index = 1;

    indexedGraph.push_back(parent);

    left->parent = parent;
    right->parent = parent;

    left->left = NULL;
    left->right = NULL;

    right->left = NULL;
    right->right = NULL;

    left->symbol = 255;
    left->symbolExtended = 1;

    right->symbol = 255;
    right->symbolExtended = 2;

    left->nrOfApp = 1;
    right->nrOfApp = 1;

    left->index = 2;
    right->index = 3;

    indexedGraph.push_back(left);
    indexedGraph.push_back(right);

    symbolMap[right->symbol + right->symbolExtended].first = right;
    symbolMap[right->symbol + right->symbolExtended].second = "1";

    symbolMap[left->symbol + left->symbolExtended].first = left;
    symbolMap[left->symbol + left->symbolExtended].second = "0";


    return parent;

}

/**
* find symbol in alphabet map, if it is first occurence for symbol return a reference for ESC symbol
*/
GraphSearch findSymbol(unsigned char symbol) {
    GraphSearch result;
    if (symbolMap.count(symbol) > 0) {
        result.code = symbolMap[symbol].second;
        result.type = true;
        result.reference = symbolMap[symbol].first;
    } else {
        result.code = symbolMap[ESC_VALUE].second;
        result.type = false;
        result.reference = symbolMap[ESC_VALUE].first;
    }
    return result;

}

void updateSymbol(GraphSearch result) {
    (result.reference)->nrOfApp = (result.reference)->nrOfApp + 1;
}

void addSymbol(GraphSearch result, unsigned char symbol) {
    GraphTree *left, *right;


    left = new GraphTree;
    left->nrOfApp = 1;
    left->right = NULL;
    left->left = NULL;
    left->symbol = symbol;
    left->symbolExtended = 0;
    left->parent = result.reference;
    left->index = (result.reference)->index + 1;

    right = new GraphTree;
    right->nrOfApp = 1;
    right->right = NULL;
    right->left = NULL;
    right->symbol = 255;
    right->symbolExtended = 2;
    right->parent = result.reference;
    right->index = (result.reference)->index + 2;


    (result.reference)->left = left;
    (result.reference)->right = right;
    (result.reference)->nrOfApp = 2;
    (result.reference)->symbol = 0;
    (result.reference)->symbolExtended = 0;

    indexedGraph.push_back(left);
    indexedGraph.push_back(right);


    symbolMap[ESC_VALUE].first = right;


}

string getCode(GraphTree *node) {
    string result;
    result = "";
    if (node->index % 2 == 0 && node->parent != NULL) {
        result = "0";
    } else if(node->parent != NULL) {
        result = "1";
    }

    if (node->parent != NULL) {
        return getCode(node->parent) + result;
    } else {
        return result;
    }
}


void displayIndexedGraph() {
    GraphIndex::size_type iterator;
    for (iterator = 0; iterator < indexedGraph.size(); ++iterator) {
        std::cout << ((indexedGraph[iterator])->symbol + (indexedGraph[iterator])->symbolExtended) << " " << (indexedGraph[iterator])->index << " -> " << iterator + 1 << " " << (indexedGraph[iterator])->nrOfApp << "\n";
    }
    std::cout << "\n\n";
}

void displayGraph(GraphTree *root) {
    if ((root->symbol + root->symbolExtended) == 0) {
        std::cout << "Pondere: " << root->nrOfApp << "\n";
        displayGraph(root->left);
        displayGraph(root->right);
    } else {
        std::cout << "Simbol: " << (root->symbol + root->symbolExtended) << "\n";
    }


}

void balanceGraph(GraphTree *node) {
    GraphTree *aux, *changeAux, *parent;
    GraphIndex::size_type iterator, auxIndex;

    for (iterator = node->index - 1; iterator > 0; iterator--) {
        if ((indexedGraph[iterator - 1])->nrOfApp >= node->nrOfApp) {
            break;
        }
    }
    if (iterator == node->index - 1) {
        node->parent->nrOfApp++;
        if (node->parent->parent != NULL) {
            balanceGraph(node->parent);
        }
        return void();
    }

    if (iterator == node->index - 2) {
        (indexedGraph[iterator - 1])->right = indexedGraph[iterator];
        (indexedGraph[iterator - 1])->left = node;
        indexedGraph[iterator]->index++;
        node->index--;
        indexedGraph[iterator] = node;
        indexedGraph[iterator + 1] = (indexedGraph[iterator - 1])->right;
        balanceGraph(node);
        return void();
    }

    if ((iterator - (node->index - 2)) > 0 && node->index - 2 >= 0) {
        aux = indexedGraph[iterator];
        auxIndex = node->index;
        node->index = aux->index;
        aux->index = auxIndex;
        indexedGraph[iterator] = node;

        parent = aux->parent;
        changeAux = node->parent;
        if (iterator % 2 == 1) {
            parent->left = node;
        } else {
            parent->right = node;
        }
        node->parent = parent;
        aux->parent = changeAux;

        indexedGraph[auxIndex - 1] = aux;
        if (auxIndex % 2 == 0) {
            changeAux->left = aux;
        } else {
            changeAux->right = aux;
        }
        balanceGraph(indexedGraph[iterator]);
        //return void();
    }
    //return void();
}

/**
* Update symbol map
*/
void updateMap() {
    int symbol;
    GraphIndex::size_type iterator;
    for (iterator = 0; iterator < indexedGraph.size(); iterator++) {
        symbol = indexedGraph[iterator]->symbolExtended + indexedGraph[iterator]->symbol;
        if (symbol > 0) {
            symbolMap[symbol].first = indexedGraph[iterator];
        }
    }
}


void encodeSymbol(unsigned char symbol, GraphTree *parent) {
    GraphSearch result;

    result = findSymbol(symbol);
    if (result.type == true) {
        updateSymbol(result);
    } else {
        addSymbol(result, symbol);
    }
    balanceGraph(result.reference);
    updateMap();


}

void swapChild(GraphTree *root) {
    GraphTree *aux;
    aux = root->left;
    root->left = root->right;
    root->right = aux;
}

void balanceTree(GraphTree *root) {

    if ((root->symbol + root->symbolExtended) == 0) {
        if (root->left->nrOfApp < root->right->nrOfApp) {
            swapChild(root);
        }
    }
}

void encodeFile() {


}

void compressFile(std::string fileName) {

    GraphTree *huffmanTree;
    huffmanTree = initGraph();
    encodeSymbol('a', huffmanTree);
    encodeSymbol('b', huffmanTree);
    encodeSymbol('c', huffmanTree);
    encodeSymbol('a', huffmanTree);
//    encodeSymbol('e', huffmanTree);
//    encodeSymbol('f', huffmanTree);
//    encodeSymbol('g', huffmanTree);
//    displayIndexedGraph();
//    displayGraph(huffmanTree);
}

void uncompressFile(std::string fileName) {

}

void displayOptions() {
    std::cout << "Alege fisierul!" << "\n\n Audio:\n";
    std::cout << "\t1.)instr_01.wav\n";
    std::cout << "\t2.)sound_01.wav\n";
    std::cout << "\t3.)speech_01.wav\n";
    std::cout << "\nDocuments:\n";
    std::cout << "\t4.)Documentatie_UMAPID.doc\n";
    std::cout << "\t5.)Documentatie_UMAPID.pdf\n";
    std::cout << "\t6.)Prefata_Undine.txt\n";
    std::cout << "\t7.)show_audio.m\n";
    std::cout << "\t8.)Y04.M\n";
    std::cout << "\nExecutables:\n";
    std::cout << "\t9.)KARMA_DATA482#1_5_V7.mat\n";
    std::cout << "\t10.)quartz.dll\n";
    std::cout << "\t11.)WinRar.exe\n";
    std::cout << "\t12.)WINZIP32.EXE\n";


}

std::ifstream::pos_type filesize(std::string fileName) {
    std::ifstream in(fileName.c_str(), std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void displayInformations(std::string fileName) {
    std::ifstream::pos_type compressedFileSize, fileSize, uncompressedFileSize;
    std::string compressedFileName, uncompressedFileName;

    compressedFileName = fileName + ".psh";
    uncompressedFileName = fileName + ".pshu";

    compressedFileSize = filesize(compressedFileName);
    fileSize = filesize(fileName);
    uncompressedFileSize = filesize(uncompressedFileName);

    std::cout << "Norma: " << (fileSize - uncompressedFileSize) * (fileSize - uncompressedFileSize) << "\n";
    std::cout << "Rata compresie: " << (1 - ((float) compressedFileSize) / ((float) uncompressedFileSize)) * 100 << "%\n";
    std::cout << "Factor compresie:  " << (((float) compressedFileSize) / ((float) fileSize)) * 100 << "%\n";


}


int main() {

    std::vector<std::string> files = {
            ".\\audio\\instr_01.wav",
            ".\\audio\\sound_01.wav",
            ".\\audio\\speech_01.wav",
            ".\\documents\\Documentatie_UMAPID.doc",
            ".\\documents\\Documentatie_UMAPID.pdf",
            ".\\documents\\Prefata_Undine.txt",
            ".\\documents\\show_audio.m",
            ".\\documents\\Y04.M",
            ".\\executables\\KARMA_DATA482#1_5_V7.mat",
            ".\\executables\\quartz.dll",
            ".\\executables\\WinRar.exe",
            ".\\executables\\WINZIP32.EXE",
            ".\\documents\\input.txt",
            "D:\\input.txt",
    };

    int option;
    //displayOptions();
    //std::cout << "Select file!..\n";
    //std::cin >> option;
//    if (option < 1 && option > 14) {
//        std::cout << "Invalid option!";
//        return 0;
//    }
    auto startC = std::chrono::high_resolution_clock::now();


    compressFile(files[2]);
    auto endC = std::chrono::high_resolution_clock::now();
    auto timeC = endC - startC;
    std::cout << "Compressed time: " << std::chrono::duration_cast<miliseconds_type>(timeC).count() << " miliseconds.\n";

    auto startU = std::chrono::high_resolution_clock::now();
//    uncompressFile(files[option - 1]);

    auto endU = std::chrono::high_resolution_clock::now();
    auto timeU = endU - startU;
    std::cout << "Uncompressed time: " << std::chrono::duration_cast<miliseconds_type>(timeU).count() << " miliseconds.\n";
//    displayInformations(files[option - 1]);

    //std::cin >> option;

    return 0;
}