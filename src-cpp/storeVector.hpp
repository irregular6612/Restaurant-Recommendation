#ifndef STOREVECTOR_H
#define STOREVECTOR_H

#include <string>
#include<vector>

class menu{
public:
    virtual std::string getMenuName() const = 0;
    virtual std::string getMenuPrice() const = 0;
    virtual int getMenuPriceValue() const = 0;
};

class restaurantMenu : public menu{
private:
    std::string menuName;
    std::string menuPrice;
    int menuPriceVal;
public:
    restaurantMenu(std::string _name, std::string _price) : menuName(_name), menuPrice(_price){ formatPrice(); }
    ~restaurantMenu(){}
    std::string getMenuName() const override{ return menuName; }
    std::string getMenuPrice() const override{ return menuPrice; }
    int getMenuPriceValue() const override{ return menuPriceVal; }
    void formatPrice(){
        int indexOfComma = menuPrice.find(',');
        try{
            menuPriceVal = std::stoi(menuPrice.substr(0,indexOfComma) + menuPrice.substr(indexOfComma+1));
        }catch(const std::exception& e){
            menuPriceVal = -1;
            //std::cerr << e.what() << '\n';
        }
    }
};
//----------------------------------------------------------------------------------------------------

enum storeCategory{ KOREAN, CHINESE, JAPANESE, CAFE, ITALIAN, WESTERN, FASTFOOD, BOONSIK, ETC };

class menuVector{
public:
    virtual std::string getName() const = 0;
    virtual std::string getTel() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::string getReview() const = 0;
    virtual int getReviewVal() const = 0;
    virtual bool hasMenu(std::string menuQuery) const = 0;
    virtual menu* getMenu(int index) const = 0;
    virtual menu* getMenu(std::string menuQuery) const = 0;
    virtual std::vector<menu*> allMenu()const = 0;
    virtual void addMenu(menu* _menu) = 0;
};

class store: public menuVector{
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
    std::string getName() const override{ return name; }
    void setTel(std::string _new){ tel = _new; }
    std::string getTel() const override{ return tel; }
    void setCatergory(std::string _new){ category = _new; }
    std::string getCategory() const override{ return category; }
    void setReview(std::string _review){ reviewCount = _review; formatReview(); } 
    std::string getReview() const override{ return reviewCount; }
    void formatReview(){
        try{
            reviewVal= std::stoi(reviewCount);
        }catch(const std::exception& e){
            reviewVal = 0;
            //std::cerr << e.what() << '\n'; -> no reivew count.
        }
    }
    int getReviewVal() const override{ return reviewVal; }
    //function overloading
    menu* getMenu(int index) const override{ return menus[index]; }
    menu* getMenu(std::string menuQuery) const override{
        for(menu* _menu : menus){
            if(_menu -> getMenuName().find(menuQuery) != std::string::npos){
                return _menu;
            }
        }
        return nullptr;
    }
    
    int menuNum() const{ return menus.size(); }
    std::vector<menu*> findAllMenu(std::string menuQuery) const{
        std::vector<menu*> targetMenu;
        for(menu* _menu : menus){
            if(_menu -> getMenuName().find(menuQuery) != std::string::npos){
                targetMenu.push_back(_menu);
            }
        }
        return targetMenu;
    }
    bool hasMenu(std::string menuQuery) const override{
        bool storeHasMenu = false;
        for(menu* _menu : menus){
            if(_menu -> getMenuName().find(menuQuery) != std::string::npos){
                storeHasMenu = true;
                break;
            }
        }
        return storeHasMenu;
    }
    std::vector<menu*> allMenu()const{ return menus; }
    //영업점의 영업 상태는 "영업 중, 영업 전, 영업 종료, BLANK, 24시간 영업"
    void addMenu(menu* _menu){ menus.push_back(_menu); }
};

//-------------------------------------------------------------------------------------------------

class storeVector{
private:
    enum order{ DEFAULT, PRICE, STARS};
    order sortStd;
    std::string menuInput;
    std::string storeInput;
    std::vector<menuVector*> stores;
    bool compareDefault(menuVector* a, menuVector* b){ return true; }
    bool comparePrice(menuVector* a, menuVector* b){
        return (a -> getMenu(menuInput) -> getMenuPriceValue()) >= ( b -> getMenu(menuInput) -> getMenuPriceValue()) ? true : false;
    }
    bool compareReview(menuVector* a, menuVector* b){
        return (a -> getReviewVal()) >= ( b -> getReviewVal()) ? true : false;
    }
public:
    storeVector(): sortStd(DEFAULT){}
    ~storeVector(){}
    void setOrder(int x){ sortStd = static_cast<order>(x); }
    int getOrder() const{ return static_cast<int>(sortStd); }


    std::vector<menuVector*> findStore2Menu(std::string menuQuery, int x= 0){
        
        setOrder(static_cast<order>(x));
        //cant const since below line.
        menuInput = menuQuery;
        std::vector<menuVector*> targetStore;

        for(menuVector* _store : stores){
            if(_store -> hasMenu(menuQuery)){
                //
                if(sortStd == DEFAULT || targetStore.size() == 0){
                    targetStore.push_back(_store);
                //가격 정렬 수정 필요.......................................................................
                //가격이 싼게 앞으로 -> idx가 증가하는데, 언제까지? -> 
                }else if(sortStd == PRICE){
                    for(int i=0;i != targetStore.size();i++){
                        if((_store -> getMenu(menuQuery) -> getMenuPriceValue()) < (targetStore[i] -> getMenu(menuQuery) -> getMenuPriceValue())){
                            targetStore.insert(targetStore.begin()+i, _store);
                            break;
                        }
                    }
                }else{  //sort order with review count.
                    for(int i=0; i != targetStore.size();i++){
                        if(_store -> getReviewVal() > targetStore[i] -> getReviewVal()){
                            targetStore.insert(targetStore.begin()+i,_store);
                            break;
                        }
                    }
                }
                //targetStore.push_back(_store);
            }
        }
        return targetStore;
    }
    menuVector* findStore2Name(std::string storeName){
        //cant const since below line.
        storeInput = storeName;
        for(menuVector* _store : stores){
            if(!(_store -> getName().compare(storeName))){
                return _store;
            }
        }
        return nullptr;
    }
    void insertStore(menuVector* s){
        if(findStore2Name(s -> getName()) == nullptr){
            stores.push_back(s);
        }
    }
};

#endif