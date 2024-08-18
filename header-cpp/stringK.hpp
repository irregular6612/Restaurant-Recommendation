#ifndef STRINGK_H
#define STRINGK_H

#include <string>

class StringK {
private:
    std::string content;
    int koreans;    //한글 글자 수 피자 -> 2, 햄버거 -> 3
public:
    StringK(std::string _content);
    int size();
    void setString(std::string _string);
    std::string getStr() const;
    //check how many korean-type char in string
    int howManyKoreans();
    int Koreans() const;
};

#endif