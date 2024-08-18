#include <iostream>
#include <exception>
//#include <string>
#include <iomanip>
#include "../header-cpp/fileHandler.hpp"

FileHandler::FileHandler(std::string filepath) : filePath(filepath), fileState(CLOSE), curLineNum(0), endLineNum(0) {

    file.open(filePath);
    if (file.fail())
        std::cout << "Cannot open inputfile" << std::endl;
    else
        fileState = OPEN;

}

FileHandler::~FileHandler() {
    file.close();
}

std::string FileHandler::getToken() const { return  this->token; }

std::string FileHandler::getLine() const { return this->line; }

bool FileHandler::readLine() {
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

bool FileHandler::readToken(char delimeter) {
    if (!line_buffer.eof()) {
        std::getline(line_buffer, token, delimeter);
        //line_buffer >> token;
        return true;
    }
    else {
        return false;
    }
}

void FileHandler::newLine() { curLineNum++; file << std::endl; }

void FileHandler::writeToken(std::string content) { file << content; }

void FileHandler::moveFront() {
    curLineNum = 0;
    file.clear();
    file.seekg(0, std::ios::beg);
}

void FileHandler::erase() {
    curLineNum = 0;
    file.close();
    std::string commandline = "cat /dev/null >" + filePath;
    system(commandline.c_str());
    file.open(filePath);
}

void FileHandler::reopen() {
    if (file.is_open()) {
        curLineNum = 0;
        file.close();
        file.open(filePath);
    }
}

int FileHandler::getCurLineNum() const{ return curLineNum; }

int FileHandler::getEndLineNum() const{ return endLineNum; }

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
