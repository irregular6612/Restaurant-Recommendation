#include <iostream>
#include <memory>
#include "browser.hpp"
#include "fileHandler.hpp"
#include "storeVector.hpp"
#include "stringK.hpp"

#define DEBUG 0

//g++ -std=c++20 -stdlib=libc++ $fileName -o $fileNameWithoutExt

// data part...
class StoreModifier{
private:
public:
    Menu* createMenu(FileHandler& db, StoreVector& storeVector) {
        std::string menuInfo = db.getToken();
        
        //remove the unless blank.
        if (menuInfo.back() == ' ') menuInfo.pop_back();
        if (menuInfo.front() == ' ') {
            std::reverse(menuInfo.begin(), menuInfo.end());
            menuInfo.pop_back();
            std::reverse(menuInfo.begin(), menuInfo.end());
        }
            
        //find the price start point start with the end of menuInfo.
        int priceStartIndex = findIndexBack(menuInfo, ' ', menuInfo.size()-1);
            
        //consider the blank
        int nameEndIndex = priceStartIndex -1;
        std::string menuName = menuInfo.substr(0, nameEndIndex);
        //if there aren't certain value of price, is stores 변동가격(업주문의)
        std::string menuPrice = menuInfo.substr(priceStartIndex);

        return new RestaurantMenu(menuName, menuPrice);
    }
    Store* createStore(FileHandler& db, StoreVector& storeVector) {
        db.readToken(); //read the storeName

        Store* store = new Store(db.getToken());    //make a store pointer with its name
        db.readToken();//read the category
        store->setCatergory(db.getToken());

        db.readLine();  //read new line
        db.readToken(); //read Telinfo
        store->setTel(db.getToken());  //set tel.
        db.readToken(); //read reviewCount
        store->setReview(db.getToken());  //set reviewCount.
        return store;
    }
    void formatData(FileHandler& db, StoreVector& storeVector) {
        while (db.readLine()) {
            if (db.getLine() == "") { //if there is a blank line -> new storeInfo starting next line
                db.readLine();  //thus, read one more line.
            }
            Store* store = createStore(db, storeVector);    

            db.readLine();  //read the menuInfo line.. ex -> 고등어조림 15,000 | 갈치조림 18,000 | 쌈밥 10,000 | 생선 모둠구이 15,000
            while (db.readToken('|')) {   // delimeter is |
                store->addMenu(createMenu(db, storeVector));
            }
            if (storeVector.findStoreWithName(store->getName()).empty()) {
                storeVector.insertStore(store);
            }
        }
    }
    void modifyData (FileHandler& db, MenuVector& newStore, Menu& newMenu, MenuVector* oldStore) {
        FileHandler temp("dataset/temp.txt");
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
            if(!newline.substr(0, (oldStore -> getName()).size()).compare(oldStore -> getName())){    //find exact store info in txt.
                //if this is note what we wanted.. -> pass

                //replace the store name
                std::string targetString = oldStore -> getName();
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
                targetString = oldStore -> getMenu(0) -> getMenuName();
                stringPos = db.getToken().find(targetString);
                
                newline.replace(stringPos, newline.find('|',stringPos) - stringPos, newMenu.getMenuName() +" "+  newMenu.getMenuPrice());
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
            if (temp.getCurLineNum() + 1 != db.getEndLineNum()){
                temp.newLine();
                temp.newLine();
            }
        }
        
        int endDataLineNum = db.getEndLineNum();
        db.erase();
        temp.moveFront();
        while (temp.readLine()) {
            db.writeToken(temp.getLine());
            
            if (db.getCurLineNum() + 1 !=  endDataLineNum){
                db.newLine();
            }
            
        }
        temp.erase();
        db.reopen();
    }
};

class AccountModifier {
private:
public:
    AccountModifier() {}
    Account* createAccount(FileHandler& db, AccountStorage& accountStorage) {
        db.readToken();
        std::string accountType =  db.getToken();
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
    void formatData(FileHandler& db, AccountStorage& accountStorage) {
        while (db.readLine()) {
           Account* account = createAccount(db, accountStorage);
           accountStorage.addAccount(account);
        }   
    }
    void modifyData(FileHandler& db, AccountStorage& accountStorage, Account& newAccount) {
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
};

class orderHistoryMaker {
private:
public:
    orderHistoryMaker() {}
    Account* createAccount(FileHandler& db, AccountStorage& accountStorage) {
        db.readToken();
        std::string accountType =  db.getToken();
        db.readToken();
        std::string id = db.getToken();
        db.readToken();
        std::string pw = db.getToken();

        if (!accountType.compare("ADMIN")) {
            return new Account(ADMIN, id, pw);
        }
        else {
            return new Account(USER,id,pw);
        }
    }
    void formatData(FileHandler& db, AccountStorage& accountStorage) {
        while (db.readLine()) {
           Account* account = createAccount(db, accountStorage);
           accountStorage.addAccount(account);
        }   
    }
};


//class implementation parts.
//main menus -> all functions below can be overwrite an action function

//if the function return true, it means need new page else, remove current page.
//every function keep its page by using cin.
//if user gives any input w/o \n, -> terminate current page and return false.

class SignInPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    AccountModifier& accountModifier;
public:
    SignInPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier) :db(db), accountStorage(accountStorage), accountModifier(accountModifier) {}
    bool action(){
        accountModifier.formatData(db, accountStorage);
        std::string id, pw;

        std::cout << "This is Sign In page...." << std::endl << std::endl;
        std::cout << "Input the ID.." << std::endl;
        #if DEBUG == 1
            id = "admin";
            pw = "admin";
        #endif

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
};

class SignUpPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    AccountModifier& accountModifier;
public:
//여기에서 생성.
    SignUpPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier) :db(db), accountStorage(accountStorage), accountModifier(accountModifier) {}
    bool action(){
        std::cout << "This is Sign Up page...." << std::endl;
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
};

//user main menus

class SearchPage {
private:
    StoreModifier& storeModifier;
    StoreVector& storeVector;
    FileHandler& db;
public:
    SearchPage(FileHandler& db, StoreVector& storeVector, StoreModifier& storeModifier) : db(db), storeVector(storeVector), storeModifier(storeModifier) {}
    bool action(){

        //dataset part....
        storeModifier.formatData(db, storeVector);
        std::cout << "This is Search page...." << std::endl;
	
        std::string inputMenu;
        #if DEBUG == 1
            inputMenu = "1";
        #endif
        std::cout << "Search(0 to exit): 1-storeName, 2-menuName"; 
        std::cin >> inputMenu;

        if (!inputMenu.compare("0")) {    //exit.
            return false;
        }
        else if (!inputMenu.compare("1")) {
            std::cout << "Store? :";

            #if DEBUG == 1
                inputMenu = "가야돌솥밥";
            #endif

            std::cin >> inputMenu;
            int i = 1;
            for (MenuVector* store : storeVector.findStoreWithName(inputMenu)) {
                std::cout << i << "번째";
                i++;
                std::cout << "[" << store->getName() << "]\t 추천수:" << store->getReview() << std::endl;

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
                std::cout << std::endl;
            }
            
        }
        else if (!inputMenu.compare("2")) {
            std::cout << "Menu? :";
            std::cin >> inputMenu;
            std::cout << "Sort? 0: default, 1: price, 2: review : ";
            int orderinput;
            std::cin >> orderinput;
    
            int i = 0;
            for (MenuVector* store : storeVector.findStoreWithMenu(inputMenu, orderinput)) {
                std::cout << i << "번째";
                i++;
                std::cout << "[" << store->getName() << "]\t 추천수:" << store->getReview() << std::endl;

                for (Menu* menu : store->allMenu()) {
                    std::cout << "- ";
                    //한글이 cout에서 3배 크기를 차지한다. -> 초,중,종성으로 따로 저장하니.
                    StringK menuName(menu -> getMenuName());
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
                std::cout << std::endl;
            }
        }

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class OrderHistoryPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    std::string id;
public:
    OrderHistoryPage(FileHandler& db, AccountStorage& accountStorage) :db(db), accountStorage(accountStorage) {}
    bool action() {
        id = accountStorage.getAccount()->getId();
        std::cout << "This is order history page...." << std::endl;
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
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class FavoritesPage {
private:
    FileHandler& db;
    AccountStorage& accountStorage;
    std::string id;
public:
    FavoritesPage(FileHandler& db, AccountStorage& accountStorage) :db(db), accountStorage(accountStorage) {}
    bool action() {
        id = accountStorage.getAccount()->getId();
        std::cout << "This is favorite page...." << std::endl;
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
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

//-> 이거 마저하면 됨..
class CategoriesPage {
private:
public:
    bool action() {
        std::string category;
        std::cout << "This is category page...." << std::endl;
        return true;
    }
};


class AccountInfoPage {
private:
    AccountModifier& accountModifier;
    FileHandler& db;
    AccountStorage& accountStorage;
public:
    AccountInfoPage(FileHandler& db, AccountStorage& accountStorage, AccountModifier& accountModifier) :db(db), accountStorage(accountStorage), accountModifier(accountModifier) {}
    bool action() {
        std::cout << "This is account info page...." << std::endl;

        accountModifier.formatData(db, accountStorage);
    
        if (accountStorage.getAccount()->getAccountType() == ADMIN) {
            std::cout << "Account Type: Admin" << std::endl; 
        }
        else {
            std::cout << "Account Type: User" << std::endl;
        }

        std::cout << "ID: " << accountStorage.getAccount()->getId() << std::endl;
        std::cout << "PW: " << accountStorage.getAccount()->getPw() << std::endl << std::endl;

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class ModifyRestaurantPage {
private:
    FileHandler& db;
    StoreVector& storeList;
    StoreModifier& storeModifier;
public:
    ModifyRestaurantPage(FileHandler& db, StoreVector& storeList, StoreModifier& storeModifier): db(db), storeList(storeList), storeModifier(storeModifier) {}
    bool action() {
        std::cout << "This is restaurant modify page...." << std::endl;
        std::string input;

        std::vector<MenuVector*> targetStoreList;

        std::cout << "Input restaurant's name to find(0 to exit): ";
        #if DEBUG == 1
            input = "가야돌솥밥";
        #endif
        
        std::cin >> input;

        if (input.compare("0")) {
            std::string restaurantName = input;
            targetStoreList = storeList.findStoreWithName(restaurantName);

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
                #if DEBUG == 1
                    cnt = 1;
                #endif
                std::cin >> cnt;
                
                
                if(cnt == 0){
                    return false;
                }
                
                //make a old store and pass this to find a value in txt.
                MenuVector* oldStore = new Store(targetStoreList[cnt - 1]->getName());
                MenuVector& targetStore = *targetStoreList[cnt - 1];

                std::cout << "Input restaurant's name to modify(0 to exit): ";
                #if DEBUG == 1
                    restaurantName = "가야돌솥밥";
                #endif

                std::cin >> restaurantName;
                targetStore.setName(restaurantName);
                std::string menuName;
                std::string newPrice;

                if (restaurantName.compare("0")) {
                   int i = 0;

                    for(Menu* menuItem : targetStore.allMenu()){
                        std::cout << i + 1 << ". " << menuItem->getMenuName() << " " << menuItem->getMenuPrice() << std::endl;
                        i++;
                    }
                    
                    std::cout << "Number for a menu, 0 to exit : ";
                    std::cin >> input;
                    if (input.compare("0")) {
                        i = std::stoi(input);

                        //make a old menu to find in the txt file.
                        Menu& targetMenu = *(targetStore.allMenu())[i-1];
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
                        return false;
                    }
                
                }
                else {
                    return false;
                }

            }
        }
        else {
            return false;
        }

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};


//categories page menus -> concrete 하나로 통일한고 .
// 파라미터로 string주면 ㅇㅋ.

class CategoriesRestaurantsPage {
private:
    std::string category;
public:
    CategoriesRestaurantsPage(std::string category) :category(category) {}
    bool action() {
        //std::string input;
        std::cout << "This is " << category << " page...." << std::endl;
        std::cout << "press any to go back";
    
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};


//class -> Capital, obj -> small
int main() {
    //make infoMaker
    StoreModifier storeModifier;
    AccountModifier accountModifier;

    //read db and make fileHanlder to control it. 
    std::string dbPath = "./dataset/DB_stores.txt";
    FileHandler db(dbPath);
    std::string dbAccountPath = "./dataset/DB_accounts.txt";
    FileHandler dbAccount(dbAccountPath);
    std::string dbOrderHistoryPath = "./dataset/DB_history.txt";
    FileHandler dbOrderHistory(dbOrderHistoryPath);
    std::string dbFavoritesPath = "./dataset/DB_favorites.txt";
    FileHandler dbFavorites(dbFavoritesPath);


    
    //storage
    StoreVector storeVector;

    // make Admin and user account.
    AccountStorage accountStorage;
    //Account Admin(ADMIN,"admin", "admin");
    //Account User1(USER, "user", "user");
    //accountStorage.addAccount(&Admin);
    //accountStorage.addAccount(&User1);
    //accountStorage.removeAccount(&User1); -> implementation needed.

    //Structure of Main Page
    PrintOnlyPage mainPage("Main Page");
    FunctionalPage<SignInPage> signInPage("Sign In", new SignInPage(dbAccount, accountStorage, accountModifier));
    FunctionalPage<SignUpPage> signUpPage("Sign Up", new SignUpPage(dbAccount, accountStorage, accountModifier));
    mainPage.addMenu(&signInPage);
    mainPage.addMenu(&signUpPage);

    //Sturucture of User Main Page.
    PrintOnlyPage userMainPage("User Main Page");
    FunctionalPage<SearchPage> searchPage("SearchPage",new SearchPage(db, storeVector, storeModifier));
    FunctionalPage<CategoriesPage> categoriesPage("CategoriesPage",new CategoriesPage);
    FunctionalPage<OrderHistoryPage> orderHistoryPage("orderHistoryPage", new OrderHistoryPage(dbOrderHistory, accountStorage));
    FunctionalPage<FavoritesPage> favoritesPage("favoritesPage", new FavoritesPage(dbFavorites, accountStorage));
    FunctionalPage<AccountInfoPage> accountInfoPage("accountInfoPage",new AccountInfoPage(dbAccount, accountStorage, accountModifier));
    FunctionalPage<ModifyRestaurantPage> modifyRestaurantPage("Modify Restaurant",new ModifyRestaurantPage(db, storeVector, storeModifier));
    userMainPage.addMenu(&searchPage);
    userMainPage.addMenu(&categoriesPage);
    userMainPage.addMenu(&orderHistoryPage);
    userMainPage.addMenu(&favoritesPage);
    userMainPage.addMenu(&accountInfoPage);
    userMainPage.addMenu(&modifyRestaurantPage);

    //Structure of Categories Page.
    PrintOnlyPage categoryStoresPage("Category's Stores Page");
    FunctionalPage<CategoriesRestaurantsPage> koreanPage("Korean",new CategoriesRestaurantsPage("Korean"));
    categoryStoresPage.addMenu(&koreanPage);
    FunctionalPage<CategoriesRestaurantsPage> chinesePage("Chinese",new CategoriesRestaurantsPage("Chinese"));
    categoryStoresPage.addMenu(&chinesePage);
    FunctionalPage<CategoriesRestaurantsPage> japanesePage("Japanese",new CategoriesRestaurantsPage("Japanese"));
    categoryStoresPage.addMenu(&japanesePage);
    FunctionalPage<CategoriesRestaurantsPage> westernPage("Western",new CategoriesRestaurantsPage("Western"));
    categoryStoresPage.addMenu(&westernPage);
    FunctionalPage<CategoriesRestaurantsPage> fastfoodPage("Fastfood",new CategoriesRestaurantsPage("Fastfood"));
    categoryStoresPage.addMenu(&fastfoodPage);
    FunctionalPage<CategoriesRestaurantsPage> beveragePage("Beverage",new CategoriesRestaurantsPage("Beverage"));
    categoryStoresPage.addMenu(&beveragePage);
    FunctionalPage<CategoriesRestaurantsPage> dessertPage("Dessert",new CategoriesRestaurantsPage("Dessert"));
    categoryStoresPage.addMenu(&dessertPage);
    
    //Structure of Order History Page
    
    //PageController such like browser
    PageController browser(accountStorage);
    browser.addPageTemplate(&mainPage);
    browser.addPageTemplate(&userMainPage);
    browser.addPageTemplate(&categoryStoresPage);

    //execute browser
    browser.addPage(&mainPage);
    browser.run();

    return 0;
}

//admin -> event input, //아니면 할인율 정보 -> 한일율 정렬이나 추천.

//implementation 이랑 헤더랑 빼기
//site 의 구조에서 history를 분리해서 메멘토로 뺴자.
//추천 시스템인데 뭔가  interesting을 줄만한게 있냐..?
//제안점.  2. sort함수를 extend가능하게 만들어라.(별도함수 분리.) 