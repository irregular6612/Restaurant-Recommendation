#ifndef MENU_H
#define MENU_H

#include <string>

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

#endif