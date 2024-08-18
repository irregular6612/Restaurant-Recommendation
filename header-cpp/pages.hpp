#ifndef PAGES_H
#define PAGES_H

#include "browser.hpp"
#include "fileHandler.hpp"
#include "storeVector.hpp"
#include "stringK.hpp"
#include "modifier.hpp"

class SignInPage : public ActionPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    AccountModifier& accountModifier;
public:
    SignInPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier);
    bool action();
};

class SignUpPage : public ActionPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    AccountModifier& accountModifier;
public:
    SignUpPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier);
    bool action();
};

class StoreActionPage : public ActionPage {
public:
    void printAllMenus(MenuVector* store, std::string& inputMenu) const;
    void showMenuList(bool& endState, std::vector<MenuVector*> targetStoreVector, std::string& title);
};


class SearchPage : public StoreActionPage {
private:
    StoreModifier& storeModifier;
    StoreVector& storeVector;
    FileHandler& db;
public:
    SearchPage(FileHandler& db, StoreVector& storeVector, StoreModifier& storeModifier);
    bool action();
};

class OrderHistoryPage : public ActionPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    std::string id;
public:
    OrderHistoryPage(FileHandler& db, AccountStorage& accountStorage);
    bool action();
};

class FavoritesPage : public ActionPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    std::string id;
public:
    FavoritesPage(FileHandler& db, AccountStorage& accountStorage);
    bool action();
};

class CategoriesPage : public ActionPage {
private:
public:
    bool action();
};

class AccountInfoPage : public ActionPage {
private:
    AccountModifier& accountModifier;
    FileHandler& db;
    AccountStorage& accountStorage;
public:
    AccountInfoPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier);
    bool action();
};

class ModifyRestaurantPage : public ActionPage {
private:
    FileHandler& db;
    StoreVector& storeVector;
    StoreModifier& storeModifier;
    AccountStorage& accountStorage;
public:
    ModifyRestaurantPage(FileHandler& db, StoreVector& storeVector, StoreModifier& storeModifier, AccountStorage& accountStorage);
    bool action();
};

class CategoriesRestaurantsPage : public StoreActionPage {
private:
    StoreVector& storeVector;
    std::string category;
public:
    CategoriesRestaurantsPage(StoreVector& storeVector, std::string category);
    bool action();
};

class DiscountInfoPage : public StoreActionPage {
private:
    StoreVector& storeVector;
public:
    DiscountInfoPage(StoreVector& storeVector);
    bool action();
};

#endif