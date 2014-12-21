class OutputStream {
    std::string outputFileName;
    std::string buffer;
    ofstream outputFile;
    static const int MAX_BUFFER_SIZE = 8;

    void openFile();

    void writeToFile(std::string);

    void write();

public:
    void OutputStream(std::string);

    void ~OutputStream();

    void addData(std::string);
};