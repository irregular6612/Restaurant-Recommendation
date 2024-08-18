#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include "../header-cpp/pages.hpp"

//pages implementation
// data part... concrete class for each data txt files.
// it makes infomation in txt files to data i memory

//class implementation parts.
//main menus -> all functions below can be overwrite an action function

//if the function return true, it means need new page else, remove current page.
//every function keep its page by using cin.get and cin.ignore
//if user gives any input w/o \n, -> terminate current page and return false.


//below is actual pages.
//Level 1 pages -> main page.

SignInPage::SignInPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier) :db(db), accountStorage(accountStorage), accountModifier(accountModifier) {}

bool SignInPage::action() {
    accountModifier.formatData(db, accountStorage);
    std::string id, pw;

    std::cout << "Input the ID.." << std::endl;
    std::cin >> id;
    std::cout << "Input the PW.." << std::endl;
    std::cin >> pw;

    if (accountStorage.login(id, pw)) {
        std::cout << "Hello " << id << std::endl;
        return true;
    }
    else {
        std::cout << "not found... try again." << std::endl;
    }
    std::cout << "go back with press enter" << std::endl;

    //for delay
    std::cin.get();
    std::cin.ignore();
    return false;
}

SignUpPage::SignUpPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier) :db(db), accountStorage(accountStorage), accountModifier(accountModifier) {}

bool SignUpPage::action() {
    accountModifier.formatData(db, accountStorage);
    std::string id, pw;
    std::string state = "1";
    AccountType accountType;
    int buffer;
    //여기 잘못 입력하면 에러 있음. -> sdf
    do {
        try {
            std::cout << "Type of Account(0 to exit, 1 for Admin, 2 for User): ";
            std::cin >> buffer;
            if (buffer == 0) {
                break;
            }
            else if (buffer == 1) {
                accountType = ADMIN;
            }
            else if (buffer == 2) {
                accountType = USER;
            }
            else {
                throw std::invalid_argument("wrong intput.");
            }
            std::cout << "Set your ID: ";
            std::cin >> id;
            std::cout << "Set your PW: ";
            std::cin >> pw;

            accountStorage.addAccount(new Account(accountType, id, pw));

        }
        catch (const std::exception& e) {
            //std::cerr << e.what() << '\n';
            break;
        }
    } while (std::stoi(state) != 0);

    std::cout << "go back with press enter" << std::endl;

    //for delay
    std::cin.get();
    std::cin.ignore();
    return false;
}

//below is Level2 pages -> user main pages.

//Store action page is a base class for some pages which are print menus.(such like search, ModifyRestaurantPage )
//only has two functions which for print menulist 
void StoreActionPage::printAllMenus(MenuVector* store, std::string& inputMenu) const {
    for (Menu* menu : store->allMenu()) {
        std::cout << "- ";
        //한글이 cout에서 3배 크기를 차지한다. -> 초,중,종성으로 따로 저장하니.
        StringK menuName(menu->getMenuName());
        int formatInterval = 70;
        std::cout << std::left << menuName.getStr() << std::right << std::setw(formatInterval - menuName.size()) << "|";
        std::cout << std::right << std::setw(10) << menu->getMenuPrice();

        if (menu->getMenuName().find(inputMenu) != std::string::npos) {
            std::cout << "  *" << std::endl;
        }
        else {
            std::cout << std::endl;

        }
    }
}

void StoreActionPage::showMenuList(bool& endState, std::vector<MenuVector*> targetStoreVector, std::string& title) {
    int pageNum = 0;
    std::string input;
    int totalStoreNum = targetStoreVector.size();
    while (endState == false) {

        #ifdef _WIN32
            system("cls");
        #endif

        #ifdef __APPLE__
            system("clear");
        #endif

        const int pageStoreNum = 10;

        std::cout << "[" << title << "]" << std::endl;
        std::cout << "Total Stores: " << totalStoreNum << std::endl;
        int maxPageNum = (totalStoreNum % pageStoreNum) ? (totalStoreNum / pageStoreNum + 1) : (totalStoreNum / pageStoreNum);
        std::cout << "current pageNum:" << pageNum + 1 << "/" << maxPageNum << std::endl << std::endl;

        for (int i = pageNum * pageStoreNum; (i < (pageNum + 1) * pageStoreNum) && i < totalStoreNum; i++) {
            std::cout << i + 1 << "번째";
            std::cout << "[" << targetStoreVector[i]->getName() << "]\t 추천수:" << targetStoreVector[i]->getReview() << std::endl;
            if (targetStoreVector[i]->getDiscount() > 10) {
                std::cout << "Event !!!!    Discount : " << targetStoreVector[i]->getDiscount() << "%" << std::endl;
            }
            std::cout << std::endl;
            printAllMenus(targetStoreVector[i], title);
            std::cout << std::endl;
        }
        std::cout << "press 1 to next or press -1 to back" << std::endl;
        std::cout << "press 0 to exit" << std::endl;
        std::cout << "select: ";
        std::cin >> input;

        if (!input.compare("1")) {
            if (pageNum <= (totalStoreNum / pageStoreNum + 1)) {
                pageNum++;
            }
            else {
                std::cout << "no next page..." << std::endl;
                std::cin.get();
                std::cin.ignore();
            }
        }
        else if (!input.compare("-1")) {
            if (pageNum == 0) {
                std::cout << "no back page..." << std::endl;
                std::cin.get();
                std::cin.ignore();
            }
            else {
                pageNum--;
            }
        }
        else {
            std::cout << "go back with press enter" << std::endl;
            endState = true;
        }
    }
}

SearchPage::SearchPage(FileHandler& db, StoreVector& storeVector, StoreModifier& storeModifier) : db(db), storeVector(storeVector), storeModifier(storeModifier) {}

bool SearchPage::action() {
    bool endState = false;
    std::string inputMenu;
    std::cout << "Search(0 to exit): 1-storeName, 2-menuName";
    std::cin >> inputMenu;

    if (!inputMenu.compare("0")) {    //exit.
        return false;
    }
    else if (!inputMenu.compare("1")) {
        std::cout << "Store? :";
        std::cin >> inputMenu;
        std::cout << std::endl;
        int i = 1;
        showMenuList(endState, storeVector.findStoreWithName(inputMenu), inputMenu);
    }
    else if (!inputMenu.compare("2")) {
        std::cout << "Menu? :";
        std::cin >> inputMenu;
        std::cout << "Sort? 0: default, 1: price, 2: review : ";
        int orderinput;
        std::cin >> orderinput;
        showMenuList(endState, storeVector.findStoreWithMenu(inputMenu, orderinput), inputMenu);
    }
    
    //for delay
    std::cin.get();
    std::cin.ignore();
    return false;
}

OrderHistoryPage::OrderHistoryPage(FileHandler& db, AccountStorage& accountStorage) :db(db), accountStorage(accountStorage) {}

bool OrderHistoryPage::action() {
    id = accountStorage.getAccount()->getId();
    db.moveFront();
    while (db.readLine()) {
        db.readToken();
        std::string historyId = db.getToken();
        if (!historyId.compare(id)) {
            db.readToken();
            std::string date = db.getToken();
            db.readToken();
            std::string time = db.getToken();
            db.readToken();
            std::string store = db.getToken();
            db.readToken();
            std::string menu = db.getToken();
            db.readToken();
            std::string price = db.getToken();
            std::cout << date << "\t" << time << "\t" << store << "\t" << menu << "\t" << price << std::endl;
        }
    }
    std::cout << "go back with press enter" << std::endl;
    //for delay
    std::cin.get();
    std::cin.ignore();
    return false;
}

FavoritesPage::FavoritesPage(FileHandler& db, AccountStorage& accountStorage) :db(db), accountStorage(accountStorage) {}

bool FavoritesPage::action() {
    id = accountStorage.getAccount()->getId();
    db.moveFront();
    while (db.readLine()) {
        db.readToken();
        std::string favoritesId = db.getToken();
        if (!favoritesId.compare(id)) {
            db.readToken();
            std::string store = db.getToken();
            db.readToken();
            std::string category = db.getToken();
            std::cout << store << "\t" << category << std::endl;
        }
    }
    std::cout << "go back with press enter" << std::endl;
    //for delay
    std::cin.get();
    std::cin.ignore();
    return false;
}

bool CategoriesPage::action() {
    std::string category;
    return true;
}

AccountInfoPage::AccountInfoPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier) :db(db), accountStorage(accountStorage), accountModifier(accountModifier) {}

bool AccountInfoPage::action() {
    accountModifier.formatData(db, accountStorage);

    if (accountStorage.getAccount()->getAccountType() == ADMIN) {
        std::cout << "Account Type: Admin" << std::endl;
    }
    else {
        std::cout << "Account Type: User" << std::endl;
    }

    std::cout << "ID: " << accountStorage.getAccount()->getId() << std::endl;
    std::cout << "PW: " << accountStorage.getAccount()->getPw() << std::endl << std::endl;
    
    std::cout << "\n press any to go back." << std::endl;
    //for delay
    std::cin.get();
    std::cin.ignore();
    return false;
}

ModifyRestaurantPage::ModifyRestaurantPage(FileHandler& db, StoreVector& storeVector, StoreModifier& storeModifier, AccountStorage& accountStorage) : db(db), storeVector(storeVector), storeModifier(storeModifier), accountStorage(accountStorage) {}

bool ModifyRestaurantPage::action() {

    if (accountStorage.getAccount()->getAccountType() == USER) {
        std::cout << "Access denied.." << std::endl;
        std::cout << "press any to go back" << std::endl;
        std::cin.get();
        std::cin.ignore();
        return false;
    }

    std::vector<MenuVector*> targetStoreList;
    std::string input;
    std::cout << "Input restaurant's name to find(0 to exit): ";
    std::cin >> input;

    if (input.compare("0")) {
        std::string restaurantName = input;
        targetStoreList = storeVector.findStoreWithName(restaurantName);

        if (targetStoreList.empty()) { //store does not exist.
            std::cout << "no store..." << std::endl;
            std::cin.get();
            std::cin.ignore();
            return false;
        }
        else {
            std::cout << "Choose the restaurant: " << std::endl;

            int cnt = 0;
            for (MenuVector* store : targetStoreList) {
                std::cout << cnt + 1 << " " << store->getName() << std::endl;
                cnt++;
            }
            std::cin >> cnt;
            if (cnt == 0) {
                return false;
            }

            //make a old store and pass this to find a value in txt.
            MenuVector* oldStore = new Store(targetStoreList[cnt - 1]->getName());
            MenuVector& targetStore = *targetStoreList[cnt - 1];

            std::cout << "Input restaurant's name to modify(0 to exit): ";
            std::cin >> restaurantName;
            targetStore.setName(restaurantName);
            std::string menuName;
            std::string newPrice;

            std::cout << "Change discount ?? current: " << targetStore.getDiscount();
            std::cout << "\nnew Discount : ";
            int newDiscount;
            std::cin >> newDiscount;
            targetStore.setDiscount(newDiscount);

            if (restaurantName.compare("0")) {
                int i = 0;

                for (Menu* menuItem : targetStore.allMenu()) {
                    std::cout << i + 1 << ". " << menuItem->getMenuName() << " " << menuItem->getMenuPrice() << std::endl;
                    i++;
                }

                std::cout << "Number for a menu, 0 to exit : ";
                std::cin >> input;
                if (input.compare("0")) {
                    i = std::stoi(input);

                    //make a old menu to find in the txt file.
                    Menu& targetMenu = *(targetStore.allMenu())[i - 1];
                    Menu* oldMenu = new RestaurantMenu(targetMenu.getMenuName(), targetMenu.getMenuPrice());
                    oldStore->addMenu(oldMenu);

                    std::string newMenuName, newMenuPrice;
                    std::cout << "Name : ";
                    std::cin >> newMenuName;
                    targetMenu.setMenuName(newMenuName);
                    std::cout << "Price(format ex. 18,000) :";
                    std::cin >> newMenuPrice;
                    targetMenu.setMenuPrice(newMenuPrice);

                    storeModifier.modifyData(db, targetStore, targetMenu, oldStore);
                }
                else {
                    std::cout << "go back with press enter" << std::endl;
                    std::cin.get();
                    std::cin.ignore();
                    return false;
                }
            }
            else {
                std::cout << "\ngo back with press enter" << std::endl;
                std::cin.get();
                std::cin.ignore();
                return false;
            }
        }
    }
    else {
        std::cout << "\ngo back with press enter" << std::endl;
        std::cin.get();
        std::cin.ignore();
        return false;
    }
    std::cout << "\ngo back with press enter" << std::endl;
    //for delay
    std::cin.get();
    std::cin.ignore();
    return false;
}

DiscountInfoPage::DiscountInfoPage(StoreVector& storeVector) :storeVector(storeVector) {}

bool DiscountInfoPage::action() {
    std::string title = "Discount Info Page";
    //DISCOUNT == 3
    storeVector.setOrder(3);
    std::string categories[8] = { "Korean", "Chinese", "Japanese", "Western","Fastfood","Boonsik","Dessert","etc" };
    std::srand(std::time(0));
    std::vector<MenuVector*> targetStoreVector = storeVector.findStoreWithCategory(categories[std::rand() % 8]);
    storeVector.setOrder(0);
    bool endState = false;
    //pageStoreNum can't be zero -> line 
    showMenuList(endState, targetStoreVector, title);
    return false;
}

//Level 3 pages.

CategoriesRestaurantsPage::CategoriesRestaurantsPage(StoreVector& storeVector, std::string category) :storeVector(storeVector), category(category) {}

bool CategoriesRestaurantsPage::action() {
    std::vector<MenuVector*> targetStoreVector = storeVector.findStoreWithCategory(category);
    bool endState = false;
    //pageStoreNum can't be zero -> line 
    showMenuList(endState, targetStoreVector, category);
    return false;
}