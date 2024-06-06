#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>
#include <iomanip>

enum FileState { OPEN, CLOSE };
typedef std::string token;
typedef std::string lineS;
typedef std::stringstream lineSS;

class FileHandler {
private:
    std::string filePath;
    std::fstream file;
    FileState fileState;
    //char delimeter;

    std::string line;
    std::stringstream line_buffer;
    std::string token;
    int curLineNum;
    int endLineNum;

public:
    FileHandler(std::string filepath) : filePath(filepath), fileState(CLOSE), curLineNum(0), endLineNum(0) {

        file.open(filePath);
        if (file.fail())
		    std::cout << "Cannot open inputfile" << std::endl;
        else
            fileState = OPEN;

    }
    ~FileHandler() {
        file.close();
    }

    std::string getToken() const { return  this->token; }
    std::string getLine() const { return this->line; }

    bool readLine() {
        try {
            if (!std::getline(file, line)) {
                endLineNum = curLineNum;
                //std::cout << "maxline : " << endLineNum << std::endl;
                throw std::out_of_range("EOF");
            }
            curLineNum++;
            line_buffer.clear();
            line_buffer.str(line);
            //std::cout << "readline() called" << std::endl;

            return true;
        }
        catch (std::exception event) {
            //std::cout << event.what() << std::endl;
            return false;
        }
    }
    bool readToken(char delimeter = '\t') {
        if (!line_buffer.eof()) {
            std::getline(line_buffer, token, delimeter);
            //line_buffer >> token;
            return true;
        }
        else {
            return false;
        }
    }
    void newLine() { curLineNum++; file << std::endl; }
    void writeToken(std::string content) { file << content; }
    void moveFront() {
        curLineNum = 0;
        file.clear();
        file.seekg(0, std::ios::beg);
    }
    void erase() {
        curLineNum = 0;
        file.close();
        std::string commandline = "cat /dev/null >" + filePath;
        system(commandline.c_str());
        file.open(filePath);
    }
    void reopen() {
        if(file.is_open()){
            curLineNum = 0;
            file.close();
            file.open(filePath);
        }
    }
    int getCurLineNum() { return curLineNum; }
    int getEndLineNum() { return endLineNum; }
};

//access to last char to find specific delimeter
int findIndexBack(std::string _string, char delimeter, int start) {
    while (start) {
        char x = _string[start];
        if (_string[start] == delimeter) {
            break;
        }
        start--;
    }
    start++;
    return start;
}
 

#endif
