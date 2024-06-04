#ifndef STORE_H
#define STORE_H

#include <string>
#include<vector>

class info{
protected:
    virtual std::string getName() const = 0;
};

class menu : public info{
private:
    std::string name;
    std::string price;
    int priceVal;
public:
    menu(std::string _name, std::string _price) : name(_name), price(_price){ formatPrice(); }
    ~menu(){}
    std::string getName() const override{ return name; }
    std::string getPrice() const{ return price; }
    int getPriceValue() const{ return priceVal; }
    void formatPrice(){
        int indexOfComma = price.find(',');
        try{
            priceVal = std::stoi(price.substr(0,indexOfComma) + price.substr(indexOfComma+1));
        }catch(const std::exception& e){
            priceVal = -1;
            //std::cerr << e.what() << '\n';
        }
    }
};

enum storeCategory{ KOREAN, CHINESE, JAPANESE, CAFE, ITALIAN, WESTERN, FASTFOOD, BOONSIK, ETC };

class infoVector{};

class store{
private:
    std::string name;
    std::vector<menu*> menus;    
    std::string tel;
    std::string category;
    std::string reviewCount;
    int reviewVal;
public:
    store(std::string _name) : name(_name){}
    ~store(){}
    void setName(std::string _new){ name = _new; }
    std::string getName() const{ return name; }
    void setTel(std::string _new){ tel = _new; }
    std::string getTel() const{ return tel; }
    void setCatergory(std::string _new){ category = _new; }
    std::string getCategory() const{ return category; }
    void setReview(std::string _review){ reviewCount = _review; formatReview(); } 
    std::string getReview() const{ return reviewCount; }
    void formatReview(){
        try{
            reviewVal= std::stoi(reviewCount);
        }catch(const std::exception& e){
            reviewVal = 0;
            //std::cerr << e.what() << '\n'; -> no reivew count.
        }
    }
    int getReviewVal() const{ return reviewVal; }
    //function overloading
    menu* getMenu(int index) const{ return menus[index]; }
    menu* getMenu(std::string menuQuery) const{
        for(menu* _menu : menus){
            if(_menu -> getName().find(menuQuery) != std::string::npos){
                return _menu;
            }
        }
        return nullptr;
    }
    
    int menuNum() const{ return menus.size(); }
    std::vector<menu*> findAllMenu(std::string menuQuery) const{
        std::vector<menu*> targetMenu;
        for(menu* _menu : menus){
            if(_menu -> getName().find(menuQuery) != std::string::npos){
                targetMenu.push_back(_menu);
            }
        }
        return targetMenu;
    }
    bool hasMenu(std::string menuQuery) const{
        bool storeHasMenu = false;
        for(menu* _menu : menus){
            if(_menu -> getName().find(menuQuery) != std::string::npos){
                storeHasMenu = true;
                break;
            }
        }
        return storeHasMenu;
    }
    std::vector<menu*> allMenu()const{ return menus; }
    //영업점의 영업 상태는 "영업 중, 영업 전, 영업 종료, BLANK, 24시간 영업"
    void addMenu(menu* _menu){
        menus.push_back(_menu);
    }
};

#endif
