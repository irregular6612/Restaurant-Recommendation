#include<iostream>
#include<sstream>
#include<memory>
#include<iomanip>
#include<vector>
#include<fstream>

class stringK{
public:
    std::string content;
    int koreans;
public:
    stringK(std::string _content): content(_content), koreans(0){ howManyKoreans(); }
    int size(){
        return content.size() - 2*koreans;
    }
    void setString(std::string _string){
        content = _string;
        howManyKoreans();
    }
    std::string getStr() const{
        return content;
    }
    //check how many korean-type char in string
    int howManyKoreans(){
        for(char x : content){
            if(x & 0x80){   // in utf-8, infront of value is 0x80, thus bitwise and
                koreans++;
            }
        }
        koreans/=3;
        return koreans; // since one korean word has same space of three alphabets. ex) 한 -> ㅎ,ㅏ,ㄴ (3)
    }
    int Koreans() const{ return koreans; }
};

class nameId{
private:
    int id;
public:
    nameId(int id):id(id){}
    int getId() const{ return id; }
};



int main(){
    std::string a;
    std::stringstream ss;
    std::fstream file;
    file.open("dataset/temp.txt");

    while (std::getline(file,a)){
       ss << a;
       
    }
    
    while (std::getline(ss,a)){
       std::cout << a << std::endl;
    }
    return 0;
}



    //size = 11, ㄱ,ㅗ,ㄱ,ㅜ,ㅁ,ㅏ,ㄴ,[ ]
    /*
    stringK s1("고구마]");
    stringK s2("고구");
    stringK s3("감자밭이");

    int x = 20;
    std::cout << std::left;
    std::cout << std::setw(x) << "-" << "|" << std::endl;
    std::cout << s2.Koreans() << std::endl;
    std::cout << s2.content.size() << std::endl;
    std::cout << std::setw(x + s1.Koreans()) << s1.getStr() << "|" << std::endl;
    std::cout << std::setw(x + s2.Koreans()) << s2.getStr() << "|" << std::endl;
    std::cout << std::setw(x + s3.Koreans()) << s3.getStr() << "|" << std::endl;
    */
    /*
    std::string _string = "가나다";
    stringK s("가나다라[]");
    std::cout << s.size() << std::endl;
    */
    /*
    std::cout <<std::setfill('*')<< std::setw(10) << _string << std::endl;
    std::cout <<std::setfill('*')<< std::setw(10+2*3) << _string << std::endl;
    std::cout <<std::setfill('*')<< std::setw(10) << "*" << std::endl;
    */

/*
//access to last char to find where the price info is start in idx
int findIndexBack(std::string _string, char delimeter, int start){
    while (index){
        if(_string[start] == delimeter){
            break;
        }
        start--;
    }
    start++;
    return start;
}

int main(){
    std::stringstream sample_stream;
    std::string sample = "홍어삼합 38,000 | 홍어찜.무침 27,000 ";
    std::string token;
    sample_stream.str(sample);
    std::getline(sample_stream, token, '|');
    std::cout << token << std::endl;



    int point = findIndexBack(token, ' ', token.size()-2);
    std::cout << point << std::endl;
    std::string price = token.substr(point);
    point = findIndexBack(price, ',', price.size());
    price = price.substr(0,point -1) + price.substr(point);
    std::cout << std::stoi(price) << std::endl;
    //std::cout << std::stoi(token.substr(findNum(token))) << std::endl;
    return 0;
}
*/