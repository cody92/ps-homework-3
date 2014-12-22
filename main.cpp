#include <iostream>
#include <vector>
#include <map>
#include <bits/stl_algo.h>
#include <bitset>
#include <fstream>
#include <chrono>
#include "InputStream.h"
#include "OutputStream.h"
#include "HuffmanCompress.h"
#include "HuffmanDecompress.h"


using namespace std;


typedef std::chrono::duration<int, std::milli> miliseconds_type;


void compressFile(std::string fileName) {
    InputStream *inputHandler;
    OutputStream *outputHandler;
    HuffmanCompress *huffmanHandler;

    inputHandler = new InputStream(fileName);
    outputHandler = new OutputStream(fileName + ".pshd");
    huffmanHandler = new HuffmanCompress(inputHandler, outputHandler);
    huffmanHandler->process();
    delete inputHandler;
    delete outputHandler;

}

void decompressFile(std::string fileName) {
    InputStream *inputHandler;
    OutputStream *outputHandler;
    HuffmanDecompress *huffmanHandler;

    inputHandler = new InputStream(fileName+ ".pshd");
    outputHandler = new OutputStream(fileName + ".pshdd");
    huffmanHandler = new HuffmanDecompress(inputHandler, outputHandler);
    huffmanHandler->process();
    delete inputHandler;
    delete outputHandler;
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


    compressFile(files[13]);
    auto endC = std::chrono::high_resolution_clock::now();
    auto timeC = endC - startC;
    std::cout << "Compressed time: " << std::chrono::duration_cast<miliseconds_type>(timeC).count() << " miliseconds.\n";

    auto startU = std::chrono::high_resolution_clock::now();
    decompressFile(files[13]);

    auto endU = std::chrono::high_resolution_clock::now();
    auto timeU = endU - startU;
    std::cout << "Uncompressed time: " << std::chrono::duration_cast<miliseconds_type>(timeU).count() << " miliseconds.\n";
//    displayInformations(files[option - 1]);

    //std::cin >> option;

    return 0;
}