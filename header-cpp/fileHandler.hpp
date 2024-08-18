#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <sstream>

//just control the txt file. read and write.
enum FileState { OPEN, CLOSE };
typedef std::string token;
typedef std::string lineS;
typedef std::stringstream lineSS;

class FileHandler {
private:
    const std::string filePath;
    std::fstream file;
    FileState fileState;
    //char delimeter;

    std::string line;
    std::stringstream line_buffer;
    std::string token;
    int curLineNum;
    int endLineNum;

public:
    FileHandler(std::string filepath);
    ~FileHandler();

    std::string getToken() const;
    std::string getLine() const;

    bool readLine();
    bool readToken(char delimeter = '\t');
    void newLine();
    void writeToken(std::string content);
    void moveFront();
    void erase();
    void reopen();
    int getCurLineNum() const;
    int getEndLineNum() const;
};

//access to last char to find specific delimeter
int findIndexBack(std::string _string, char delimeter, int start);
 

#endif
