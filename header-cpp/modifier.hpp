#ifndef MODIFIER_H
#define MODIFIER_H

#include "storeVector.hpp"
#include "fileHandler.hpp"
#include "account.hpp"

//modifier classes
// read from the txt file and load to memory
//or can modify data and overwrite txt files.

//since these functions have only function with reference parameters, each has dependency relationship.

class StoreModifier {
private:
public:
    Menu* createMenu(FileHandler& db, StoreVector& storeVector);
    Store* createStore(FileHandler& db, StoreVector& storeVector);
    void formatData(FileHandler& db, StoreVector& storeVector);
    void modifyData(FileHandler& db, MenuVector& newStore, Menu& newMenu, MenuVector* oldStore);
};

class AccountModifier {
private:
public:
    AccountModifier();
    Account* createAccount(FileHandler& db, AccountStorage& accountStorage);
    void formatData(FileHandler& db, AccountStorage& accountStorage);
    void modifyData(FileHandler& db, AccountStorage& accountStorage, Account& newAccount);
};

class OrderHistoryMaker {
private:
public:
    OrderHistoryMaker();
    Account* createAccount(FileHandler& db, AccountStorage& accountStorage);
    void formatData(FileHandler& db, AccountStorage& accountStorage);
};
#endif 