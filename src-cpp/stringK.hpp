class StringK {
private:
    std::string content;
    int koreans;    //한글 글자 수 피자 -> 2, 햄버거 -> 3
public:
    StringK(std::string _content) : content(_content), koreans(0) { howManyKoreans(); }
    int size(){
        return content.size() - 2 * koreans;
    }
    void setString(std::string _string){
        content = _string;
        howManyKoreans();
    }
    std::string getStr() const{
        return content;
    }
    //check how many korean-type char in string
    int howManyKoreans() {
        for (char x : content) {
            if (x & 0x80) {   // in utf-8, infront of value is 0x80, thus bitwise and
                koreans++;
            }
        }
        koreans /= 3;
        return koreans; // since one korean word has same space of three alphabets. ex) 한 -> ㅎ,ㅏ,ㄴ (3)
    }
    int Koreans() const { return koreans; }
};