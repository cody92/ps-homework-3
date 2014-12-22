cmake_minimum_required(VERSION 2.8.4)
project(ps_homework_3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Huffman.h HuffmanCompress.h InputStream.h OutputStream.h Huffman.cpp HuffmanCompress.cpp InputStream.cpp OutputStream.cpp HuffmanDecompress.cpp HuffmanDecompress.h)
add_executable(ps_homework_3 ${SOURCE_FILES})