#include "../header-cpp/stringK.hpp"

StringK::StringK(std::string _content) : content(_content), koreans(0) { howManyKoreans(); }

int StringK::size() {
    return content.size() - 2 * koreans;
}

void StringK::setString(std::string _string) {
    content = _string;
    howManyKoreans();
}

std::string StringK::getStr() const {
    return content;
}

//check how many korean-type char in string

int StringK::howManyKoreans() {
    for (char x : content) {
        if (x & 0x80) {   // in utf-8, infront of value is 0x80, thus bitwise and
            koreans++;
        }
    }
    koreans /= 3;
    return koreans; // since one korean word has same space of three alphabets. ex) �� -> ��,��,�� (3)
}

int StringK::Koreans() const { return koreans; }
