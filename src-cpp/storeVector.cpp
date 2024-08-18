#include <iostream>
#include "../header-cpp/storeVector.hpp"

RestaurantMenu::RestaurantMenu(std::string _name, std::string _price) : menuName(_name), menuPrice(_price) { formatPrice(); }

RestaurantMenu::~RestaurantMenu() {}

std::string RestaurantMenu::getMenuName() const { return menuName; }

std::string RestaurantMenu::getMenuPrice() const { return menuPrice; }

int RestaurantMenu::getMenuPriceValue() const { return menuPriceVal; }

void RestaurantMenu::setMenuName(std::string menuName) { this->menuName = menuName; }

void RestaurantMenu::setMenuPrice(std::string menuPrice) { this->menuPrice = menuPrice; }

void RestaurantMenu::setMenuPriceValue(int menuPriceVal) { this->menuPriceVal = menuPriceVal; }

void RestaurantMenu::formatPrice() {
    int indexOfComma = menuPrice.find(',');
    try {
        menuPriceVal = std::stoi(menuPrice.substr(0, indexOfComma) + menuPrice.substr(indexOfComma + 1));
    }
    catch (const std::exception& e) {
        //if there not exists the price information, set val -1.
        menuPriceVal = -1;
    }
}

Store::Store(std::string name) : storeName(name), storeReviewVal(0), discountRate(0) {}

Store::~Store() {}

void Store::setName(std::string newName) { storeName = newName; }

void Store::setTel(std::string newTel) { storeTel = newTel; }

void Store::setCategory(std::string newCategory) { storeCategory = newCategory; }

void Store::setReview(std::string review) { storeReviewCount = review; formatReview(); }

void Store::setDiscount(int discountRate) { this->discountRate = discountRate; }

std::string Store::getName() const { return storeName; }

std::string Store::getTel() const { return storeTel; }

std::string Store::getCategory() const { return storeCategory; }

std::string Store::getReview() const { return storeReviewCount; }

int Store::getDiscount() const { return discountRate; }

void Store::formatReview() {
    try {
        storeReviewVal = std::stoi(storeReviewCount);
    }
    catch (const std::exception& e) {
        storeReviewVal = 0;
        //std::cerr << e.what() << '\n'; -> no reivew count.
    }
}

int Store::getReviewVal() const { return storeReviewVal; }

//function overloading

Menu* Store::getMenu(int index) const { return storeMenus[index]; }

Menu* Store::getMenu(std::string menuQuery) const {
    for (Menu* menu : storeMenus) {
        if (menu->getMenuName().find(menuQuery) != std::string::npos) {
            return menu;
        }
    }
    return nullptr;
}

int Store::menuNum() const { return storeMenus.size(); }

std::vector<Menu*> Store::findAllMenu(std::string menuQuery) const {
    std::vector<Menu*> targetMenu;
    for (Menu* menu : storeMenus) {
        if (menu->getMenuName().find(menuQuery) != std::string::npos) {
            targetMenu.push_back(menu);
        }
    }
    return targetMenu;
}

bool Store::hasMenu(std::string menuQuery) const {
    bool storeHasMenu = false;
    for (Menu* menu : storeMenus) {
        if (menu->getMenuName().find(menuQuery) != std::string::npos) {
            storeHasMenu = true;
            break;
        }
    }
    return storeHasMenu;
}

std::vector<Menu*> Store::allMenu() const { return storeMenus; }

//�������� ���� ���´� "���� ��, ���� ��, ���� ����, BLANK, 24�ð� ����"

void Store::addMenu(Menu* menu) { storeMenus.push_back(menu); }

StoreVector::StoreVector() : sortStd(DEFAULT) {}

StoreVector::~StoreVector() {}

void StoreVector::setOrder(int x) { sortStd = static_cast<SortStd>(x); }

int StoreVector::getOrder() const { return static_cast<int>(sortStd); }

std::vector<MenuVector*> StoreVector::sort(std::string menuQuery) {

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
            }
            else if (sortStd == DISCOUNT) {
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

std::vector<MenuVector*> StoreVector::findStoreWithMenu(std::string menuQuery, int x) {
    setOrder(static_cast<SortStd>(x));
    menuInput = menuQuery;
    return sort(menuQuery);
}

std::vector<MenuVector*> StoreVector::findStoreWithName(std::string storeName) {
    std::vector<MenuVector*> targetStore;
    storeInput = storeName;
    for (MenuVector* store : stores) {
        if (store->getName().find(storeName) != std::string::npos) {
            targetStore.push_back(store);
        }
    }
    return targetStore;
}

std::vector<MenuVector*> StoreVector::findStoreWithCategory(std::string storeCategory) {
    std::vector<MenuVector*> targetStore;
    storeInput = storeCategory;
    for (MenuVector* store : stores) {
        if (store->getCategory().find(storeCategory) != std::string::npos) {
            if (sortStd == DISCOUNT) {
                for (int i = 0; i != targetStore.size(); i++) {
                    if(targetStore.empty()){
                        break;
                    }
                    if (store->getDiscount() > targetStore[i]->getDiscount()) {
                        targetStore.insert(targetStore.begin() + i, store);
                        break;
                    }
                }
            }
            targetStore.push_back(store);
        }
    }
    return targetStore;
}

void StoreVector::insertStore(MenuVector* s) {
    if (findStoreWithName(s->getName()).empty()) {
        stores.push_back(s);
    }
}

bool StoreVector::isReady() {
    return !stores.empty();
}
