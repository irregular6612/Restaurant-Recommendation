#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include"../header-cpp/modifier.hpp"

Menu* StoreModifier::createMenu(FileHandler& db, StoreVector& storeVector) {
    std::string menuInfo = db.getToken();

    //remove the unless blank.
    if (menuInfo.back() == ' ') menuInfo.pop_back();
    if (menuInfo.front() == ' ') {
        std::reverse(menuInfo.begin(), menuInfo.end());
        menuInfo.pop_back();
        std::reverse(menuInfo.begin(), menuInfo.end());
    }

    //find the price start point start with the end of menuInfo.
    int priceStartIndex = findIndexBack(menuInfo, ' ', menuInfo.size() - 1);
    //consider the blank
    int nameEndIndex = priceStartIndex - 1;
    std::string menuName = menuInfo.substr(0, nameEndIndex);
    //if there aren't certain value of price, is stores 변동가격(업주문의)
    std::string menuPrice = menuInfo.substr(priceStartIndex);

    return new RestaurantMenu(menuName, menuPrice);
}

Store* StoreModifier::createStore(FileHandler& db, StoreVector& storeVector) {
    db.readToken(); //read the storeName

    Store* store = new Store(db.getToken());    //make a store pointer with its name
    db.readToken();//read the category
    store->setCategory(db.getToken());

    db.readLine();  //read new line
    db.readToken(); //read Telinfo
    store->setTel(db.getToken());  //set tel.
    db.readToken(); //read reviewCount
    store->setReview(db.getToken());  //set reviewCount.

    //since we can't get discount info, instead we take random
    
    std::srand(static_cast<unsigned int>(std::clock()));

    int virtualDiscountRate = (std::rand() % 5) * 5;
    store->setDiscount(virtualDiscountRate);
    //store->setDiscount(virtualDiscountRate);
    return store;
}

void StoreModifier::formatData(FileHandler& db, StoreVector& storeVector) {
    while (db.readLine()) {
        if (db.getLine() == "") { //if there is a blank line -> new storeInfo starting next line
            db.readLine();  //thus, read one more line.
        }
        Store* store = createStore(db, storeVector);

        db.readLine();  //read the menuInfo line.. ex -> 고등어조림 15,000 | 갈치조림 18,000 | 쌈밥 10,000 | 생선 모둠구이 15,000
        while (db.readToken('|')) {   // delimeter is |
            store->addMenu(createMenu(db, storeVector));
        }
        if (storeVector.findStoreWithName(store->getName()).empty()) {  //중복 검사하고 
            storeVector.insertStore(store);
        }
    }
}

void StoreModifier::modifyData(FileHandler& db, MenuVector& newStore, Menu& newMenu, MenuVector* oldStore) {
    /*
    Algorithm
    1. copy part of the origin data which is before data will be edited to temp file
    2. write the edited data to temp
    3. write the left part of the origin to the temp txt
    4. copy temp to origin
    5. delete temp file
    */

    FileHandler temp("dataset/temp.txt");   //temp txt file.
    Menu* targetMenu = newStore.getMenu(newMenu.getMenuName());
    targetMenu->setMenuPrice(newMenu.getMenuPrice());
    targetMenu->setMenuPriceValue(newMenu.getMenuPriceValue());
    db.moveFront();
    while (db.readLine()) {
        if (db.getLine() == "") { //if there is a blank line -> new storeInfo starting next line
            db.readLine();  //thus, read one more line.
        }

        std::string newline = db.getLine();
        //extarct the restaurant name and compare.
        if (!newline.substr(0, (oldStore->getName()).size()).compare(oldStore->getName())) {    //find exact store info in txt.
            //if this is note what we wanted.. -> pass

            //replace the store name
            std::string targetString = oldStore->getName();
            size_t stringPos = newline.find(targetString);
            newline.replace(stringPos, targetString.size(), newStore.getName());
            temp.writeToken(newline);
            temp.newLine();

            db.readLine();
            temp.writeToken(db.getLine());  //copy 2nd line.
            temp.newLine();
            db.readLine();  //read 3rd line. menu line

            newline = db.getLine();
            //replace the name and price
            targetString = oldStore->getMenu(0)->getMenuName();
            stringPos = db.getLine().find(targetString);

            std::cout << targetString << std::endl;
            //메뉴가 1개인 가게는 기호가 없다.
            newline.replace(stringPos, newline.find('|', stringPos) - stringPos, newMenu.getMenuName() + " " + newMenu.getMenuPrice());
            temp.writeToken(newline);
        }
        else {
            temp.writeToken(db.getLine());  //copy first line
            temp.newLine();
            db.readLine();
            temp.writeToken(db.getLine());  //copy 2nd line.
            temp.newLine();
            db.readLine();  //read 3rd line. menu line
            temp.writeToken(db.getLine());  //copy 3rd line.
        }
        if (temp.getCurLineNum() + 1 != db.getEndLineNum()) {
            temp.newLine();
            temp.newLine();
        }
    }

    int endDataLineNum = db.getEndLineNum();
    db.erase();
    temp.moveFront();
    while (temp.readLine()) {
        db.writeToken(temp.getLine());

        if (db.getCurLineNum() + 1 != endDataLineNum) {
            db.newLine();
        }

    }
    temp.erase();
    db.reopen();
}

AccountModifier::AccountModifier() {}

Account* AccountModifier::createAccount(FileHandler& db, AccountStorage& accountStorage) {
    db.readToken();
    std::string accountType = db.getToken();
    db.readToken();
    std::string id = db.getToken();
    db.readToken();
    std::string pw = db.getToken();

    if (!accountType.compare("ADMIN")) {
        return new Account(ADMIN, id, pw);
    }
    else {
        return new Account(USER, id, pw);
    }
}

void AccountModifier::formatData(FileHandler& db, AccountStorage& accountStorage) {
    while (db.readLine()) {
        Account* account = createAccount(db, accountStorage);
        accountStorage.addAccount(account);
    }
}

void AccountModifier::modifyData(FileHandler& db, AccountStorage& accountStorage, Account& newAccount) {
    FileHandler temp("dataset/temp.txt");
    Account* targetAccount = accountStorage.find(newAccount.getId(), newAccount.getPw());
    targetAccount->setId(newAccount.getId());
    targetAccount->setPw(newAccount.getPw());
    db.moveFront();
    while (db.readLine()) {
        db.readToken();
        std::string type = db.getToken();
        db.readToken();
        std::string id = db.getToken();
        db.readToken();
        std::string pw = db.getToken();

        if (!id.compare(newAccount.getId())) {
            temp.writeToken(type + "\t" + id + "\t" + pw + "\n");
        }
        else {
            temp.writeToken(db.getLine());
        }
    }
    db.erase();
    temp.moveFront();
    while (temp.readLine()) {
        db.writeToken(temp.getLine());
    }
    temp.erase();
}

OrderHistoryMaker::OrderHistoryMaker() {}

Account* OrderHistoryMaker::createAccount(FileHandler& db, AccountStorage& accountStorage) {
    db.readToken();
    std::string accountType = db.getToken();
    db.readToken();
    std::string id = db.getToken();
    db.readToken();
    std::string pw = db.getToken();

    if (!accountType.compare("ADMIN")) {
        return new Account(ADMIN, id, pw);
    }
    else {
        return new Account(USER, id, pw);
    }
}

void OrderHistoryMaker::formatData(FileHandler& db, AccountStorage& accountStorage) {
    while (db.readLine()) {
        Account* account = createAccount(db, accountStorage);
        accountStorage.addAccount(account);
    }
}

