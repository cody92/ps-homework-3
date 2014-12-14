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
} ShannonTree;

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

    right = new GraphTree;
    right->nrOfApp = 1;
    right->right = NULL;
    right->left = NULL;
    right->symbol = 255;
    right->symbolExtended = 2;
    right->parent = result.reference;


    (result.reference)->left = left;
    (result.reference)->right = right;
    (result.reference)->nrOfApp = 2;
    (result.reference)->symbol = 0;
    (result.reference)->symbolExtended = 0;


}

/**
* After a new symbol is added we need to update with one unit all parent nodes
*/
void updateNoOfApp(GraphTree *node) {
    GraphTree *iterator;
    iterator = node->parent;
    while (iterator != NULL) {
        node->nrOfApp++;
    }
}


void displayGraph(GraphTree *root) {
    if ((root->symbol + root->symbolExtended) == 0) {
        displayGraph(root->left);
        displayGraph(root->right);
    } else {
        std::cout << "Simbol: " << (root->symbol + root->symbolExtended) << "\n";
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

    //updateNoOfApp(result.reference);
}

void encodeFile() {


}

void compressFile(std::string fileName) {

    GraphTree *huffmanTree;
    huffmanTree = initGraph();
    displayGraph(huffmanTree);
    encodeSymbol('a', huffmanTree);
    std::cout << "\n\n";
    displayGraph(huffmanTree);
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