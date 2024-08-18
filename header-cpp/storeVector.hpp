#ifndef STOREVECTOR_H
#define STOREVECTOR_H

#include <vector>

//class of menu.(Abstract)
//since it is abstract, we can easily extend to add other type menu
class Menu{
public:
    virtual std::string getMenuName() const = 0;
    virtual std::string getMenuPrice() const = 0;
    virtual int getMenuPriceValue() const = 0;

    virtual void setMenuName(std::string) = 0;
    virtual void setMenuPrice(std::string) = 0;
    virtual void setMenuPriceValue(int) = 0;
};

//class of Restaurant menu
class RestaurantMenu : public Menu{
private:
    std::string menuName;
    std::string menuPrice;
    int menuPriceVal;
public:
    RestaurantMenu(std::string _name, std::string _price);
    ~RestaurantMenu();
    std::string getMenuName() const override;
    std::string getMenuPrice() const override;
    int getMenuPriceValue() const override;

    void setMenuName(std::string menuName) override;
    void setMenuPrice(std::string menuPrice) override;
    void setMenuPriceValue(int menuPriceVal) override;

    void formatPrice();
};
//----------------------------------------------------------------------------------------------------

class MenuVector {  //abstract of store.
public:
    virtual std::string getName() const = 0;
    virtual std::string getTel() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::string getReview() const = 0;
    virtual int getReviewVal() const = 0;
    virtual int getDiscount() const = 0;
    
    virtual void setName(std::string name) = 0;
    virtual void setTel(std::string newTel) = 0;    
    virtual void setCategory(std::string newCategory) = 0;
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
    Store(std::string name);
    ~Store();
    void setName(std::string newName) override;
    void setTel(std::string newTel) override;
    void setCategory(std::string newCategory) override;
    void setReview(std::string review) override;
    void setDiscount(int discountRate) override;

    std::string getName() const override;
    std::string getTel() const override;
    std::string getCategory() const override;
    std::string getReview() const override;
    int getDiscount() const override;
    void formatReview();
    int getReviewVal() const override;
    //function overloading
    Menu* getMenu(int index) const override;
    Menu* getMenu(std::string menuQuery) const override;
    
    int menuNum() const;
    std::vector<Menu*> findAllMenu(std::string menuQuery) const;
    bool hasMenu(std::string menuQuery) const override;
    std::vector<Menu*> allMenu() const override;
    //영업점의 영업 상태는 "영업 중, 영업 전, 영업 종료, BLANK, 24시간 영업"
    void addMenu(Menu* menu) override;
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
    StoreVector();
    ~StoreVector();
    void setOrder(int x);
    int getOrder() const;
    std::vector<MenuVector*> sort(std::string menuQuery);
    std::vector<MenuVector*> findStoreWithMenu(std::string menuQuery, int x = 0);
    std::vector<MenuVector*> findStoreWithName(std::string storeName);
    std::vector<MenuVector*> findStoreWithCategory(std::string storeCategory);
    void insertStore(MenuVector* s);
    bool isReady();
};

#endif