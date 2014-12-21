class InputStream {
    std::string inputFileName;
    ifstream inputFile;
    void openFile();

public:
    void InputStream(std::string);
    unsigned char getNextChar();
    void ~InputStream();
};