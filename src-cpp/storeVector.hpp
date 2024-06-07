#ifndef STOREVECTOR_H
#define STOREVECTOR_H

#include <string>
#include <vector>

class Menu{
public:
    virtual std::string getMenuName() const = 0;
    virtual std::string getMenuPrice() const = 0;
    virtual int getMenuPriceValue() const = 0;

    virtual void setMenuName(std::string) = 0;
    virtual void setMenuPrice(std::string) = 0;
    virtual void setMenuPriceValue(int) = 0;
};

class RestaurantMenu : public Menu{
private:
    std::string menuName;
    std::string menuPrice;
    int menuPriceVal;
public:
    RestaurantMenu(std::string _name, std::string _price) : menuName(_name), menuPrice(_price){ formatPrice(); }
    ~RestaurantMenu(){}
    std::string getMenuName() const override{ return menuName; }
    std::string getMenuPrice() const override{ return menuPrice; }
    int getMenuPriceValue() const override{ return menuPriceVal; }

    void setMenuName(std::string menuName) override{ this -> menuName = menuName; }
    void setMenuPrice(std::string menuPrice) override{ this -> menuPrice = menuPrice; }
    void setMenuPriceValue(int menuPriceVal) override{this -> menuPriceVal = menuPriceVal; }

    void formatPrice() {
        int indexOfComma = menuPrice.find(',');
        try {
            menuPriceVal = std::stoi(menuPrice.substr(0, indexOfComma) + menuPrice.substr(indexOfComma + 1));
        }
        catch (const std::exception& e) {
            //if there not exists the price information, set val -1.
            menuPriceVal = -1;
        }
    }
};
//----------------------------------------------------------------------------------------------------

enum StoreCategory { KOREAN, CHINESE, JAPANESE, CAFE, WESTERN, FASTFOOD, BOONSIK, ETC };

class MenuVector {
public:
    virtual std::string getName() const = 0;
    virtual std::string getTel() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::string getReview() const = 0;
    virtual int getReviewVal() const = 0;
    virtual int getDiscount() const = 0;
    
    virtual void setName(std::string name) = 0;
    virtual void setTel(std::string newTel) = 0;    
    virtual void setCatergory(std::string newCategory) = 0;
    virtual void setReview(std::string review) = 0;
    virtual void setDiscount(int discountRate) = 0;

    virtual void addMenu(Menu* menu) = 0;
    virtual bool hasMenu(std::string menuQuery) const = 0;
    virtual Menu* getMenu(int index) const = 0;
    virtual Menu* getMenu(std::string menuQuery) const = 0;
    virtual std::vector<Menu*> allMenu()const = 0;
};

class Store : public MenuVector {
private:
    std::string storeName;
    std::vector<Menu*> storeMenus;
    std::string storeTel;
    std::string storeCategory;
    std::string storeReviewCount;
    int storeReviewVal;
    int discountRate;
public:
    Store(std::string name) : storeName(name), storeReviewVal(0), discountRate(0) {}
    ~Store() {}
    void setName(std::string newName) override{ storeName = newName; }
    void setTel(std::string newTel) override { storeTel = newTel; }
    void setCatergory(std::string newCategory) override { storeCategory = newCategory; }
    void setReview(std::string review) override { storeReviewCount = review; formatReview(); }
    void setDiscount(int discountRate) override { this -> discountRate = discountRate; }

    std::string getName() const override { return storeName; }
    std::string getTel() const override { return storeTel; }
    std::string getCategory() const override { return storeCategory; }
    std::string getReview() const override { return storeReviewCount; }
    int getDiscount() const override { return discountRate; }
    void formatReview() {
        try {
            storeReviewVal= std::stoi(storeReviewCount);
        }
        catch (const std::exception& e) {
            storeReviewVal = 0;
            //std::cerr << e.what() << '\n'; -> no reivew count.
        }
    }
    int getReviewVal() const override { return storeReviewVal; }
    //function overloading
    Menu* getMenu(int index) const override { return storeMenus[index]; }
    Menu* getMenu(std::string menuQuery) const override {
        for (Menu* menu : storeMenus) {
            if (menu->getMenuName().find(menuQuery) != std::string::npos) {
                return menu;
            }
        }
        return nullptr;
    }
    
    int menuNum() const { return storeMenus.size(); }
    std::vector<Menu*> findAllMenu(std::string menuQuery) const {
        std::vector<Menu*> targetMenu;
        for (Menu* menu : storeMenus) {
            if (menu->getMenuName().find(menuQuery) != std::string::npos) {
                targetMenu.push_back(menu);
            }
        }
        return targetMenu;
    }
    bool hasMenu(std::string menuQuery) const override {
        bool storeHasMenu = false;
        for (Menu* menu : storeMenus) {
            if (menu->getMenuName().find(menuQuery) != std::string::npos) {
                storeHasMenu = true;
                break;
            }
        }
        return storeHasMenu;
    }
    std::vector<Menu*> allMenu() const override { return storeMenus; }
    //영업점의 영업 상태는 "영업 중, 영업 전, 영업 종료, BLANK, 24시간 영업"
    void addMenu(Menu* menu) override { storeMenus.push_back(menu); }
};

//-------------------------------------------------------------------------------------------------

class StoreVector {
private:
    enum SortStd { DEFAULT, PRICE, REVIEW , DISCOUNT };
    SortStd sortStd;
    std::string menuInput;
    std::string storeInput;
    std::vector<MenuVector*> stores;
public:
    StoreVector() : sortStd(DEFAULT) {}
    ~StoreVector() {}
    void setOrder(int x) { sortStd = static_cast<SortStd>(x); }
    int getOrder() const { return static_cast<int>(sortStd); }
    std::vector<MenuVector*> sort(std::string menuQuery) {
        
        std::vector<MenuVector*> targetStore;

        for (MenuVector* store : stores) {
            if (store->hasMenu(menuQuery)) {
                if (targetStore.size() == 0) {
                    targetStore.push_back(store);
                }
                else if (sortStd == PRICE) {
                    for (int i = 0; i != targetStore.size(); i++) {
                        if ((store->getMenu(menuQuery)->getMenuPriceValue()) < (targetStore[i]->getMenu(menuQuery)->getMenuPriceValue())) {
                            targetStore.insert(targetStore.begin() + i, store);
                            break;
                        }
                    }
                }
                else if (sortStd == REVIEW) {  //sort order with review count.
                    for (int i = 0; i != targetStore.size(); i++) {
                        if (store->getReviewVal() > targetStore[i]->getReviewVal()) {
                            targetStore.insert(targetStore.begin() + i, store);
                            break;
                        }
                    }
                }else if (sortStd == DISCOUNT) {
                    for (int i = 0; i != targetStore.size(); i++) {
                        if (store->getDiscount() > targetStore[i]->getDiscount()) {
                            targetStore.insert(targetStore.begin() + i, store);
                            break;
                        }
                    }
                }
                else {  //DEFAULT
                    targetStore.push_back(store);
                }
            }
        }
        return targetStore;
    }
    std::vector<MenuVector*> findStoreWithMenu(std::string menuQuery, int x = 0) {
        setOrder(static_cast<SortStd>(x));
        menuInput = menuQuery;
        return sort(menuQuery);
    }
    std::vector<MenuVector*> findStoreWithName(std::string storeName) {
        std::vector<MenuVector*> targetStore;
        storeInput = storeName;
        for (MenuVector* store : stores) {
            if (store->getName().find(storeName) != std::string::npos) {
                targetStore.push_back(store);
            }
        }
        return targetStore; 
    }
    std::vector<MenuVector*> findStoreWithCategory(std::string storeCategory) {
        std::vector<MenuVector*> targetStore;
        storeInput = storeCategory;
        for (MenuVector* store : stores) {
            if (store->getCategory().find(storeCategory) != std::string::npos) {
                targetStore.push_back(store);
            }
        }
        return targetStore; 
    }
    void insertStore(MenuVector* s) {
        if (findStoreWithName(s->getName()).empty()) {
            stores.push_back(s);
        }
    }
    bool isReady(){
        return !stores.empty();
    }
};

#endif