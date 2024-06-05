#include<iostream>
#include<memory>
#include"browser.hpp"
#include"fileHandler.hpp"
#include "storeVector.hpp"
#include "stringK.hpp"

// data part...
class InfoMaker{
private:
public:
    menu* createMenu(fileHandler& DB, storeVector& storeList){
        std::string menuInfo = DB.getToken();
        
        //remove the unless blank.
        if(menuInfo.back() == ' ') menuInfo.pop_back();
        if(menuInfo.front() == ' '){
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

        return new restaurantMenu(menuName, menuPrice);
    }
    store* createStore(fileHandler& DB, storeVector& storeList){
        DB.readToken(); //read the storeName

        store* s = new store(DB.getToken());    //make a store pointer with its name
        DB.readToken();//read the category
        s -> setCatergory(DB.getToken());

        DB.readLine();  //read new line
        DB.readToken(); //read Telinfo
        s -> setTel(DB.getToken());  //set tel.
        DB.readToken(); //read reviewCount
        s -> setReview(DB.getToken());  //set reviewCount.
        return s;
    }

    void formatData(fileHandler& DB, storeVector& storeList){
        while (DB.readLine()){

            if(DB.getLine() == ""){ //if there is a blank line -> new storeInfo starting next line
                DB.readLine();  //thus, read one more line.
            }
            store* s = createStore(DB, storeList);    

            DB.readLine();  //read the menuInfo line.. ex -> 고등어조림 15,000 | 갈치조림 18,000 | 쌈밥 10,000 | 생선 모둠구이 15,000
            while(DB.readToken('|')){   // delimeter is |
                s -> addMenu(createMenu(DB, storeList));
            }
            if(storeList.findStore2Name( s -> getName()).empty()){
                storeList.insertStore(s);
            }
        }
    }
    void modifyData(fileHandler& DB, menuVector& store, menu& newMenu){
        fileHandler temp("dataset/temp.txt");
        menu* target = store.getMenu(newMenu.getMenuName());
        target -> setMenuPrice(newMenu.getMenuPrice());
        target -> setMenuPriceValue(newMenu.getMenuPriceValue());
        DB.moveFront();
        while (DB.readLine()){
            if(DB.getLine() == ""){ //if there is a blank line -> new storeInfo starting next line
                DB.readLine();  //thus, read one more line.
            }

            if(DB.getLine().find(store.getName()) != std::string::npos){    //find exact store info in txt.
                temp.writeToken(DB.getLine() + "\n");  //copy first line
                DB.readLine();
                temp.writeToken(DB.getLine() + "\n");  //copy 2nd line.
                DB.readLine();  //read 3rd line. menu line

                std::string newline;
                while (DB.readToken('|')){
                    if(DB.getToken().find(newMenu.getMenuName()) != std::string::npos){
                        newline = newline + newMenu.getMenuName() + " "+ newMenu.getMenuPrice() + " | ";
                    }else{
                        newline = newline + DB.getToken() + " | ";
                    }
                }
                newline = newline.substr(0, newline.size() - 3);
                newline+="\n";
                temp.writeToken(newline);
            }else{
                temp.writeToken(DB.getLine() + "\n");  //copy first line
                DB.readLine();
                temp.writeToken(DB.getLine() + "\n");  //copy 2nd line.
                DB.readLine();  //read 3rd line. menu line
                temp.writeToken(DB.getLine() + "\n");  //copy 3rd line.
            }
        }
        DB.clear();
        temp.moveFront();
        while (temp.readLine()){
            DB.writeToken(temp.getLine());
        }
        temp.clear();
    }
};

class accountMaker{
private:
public:
    accountMaker(){}
    Account* createAccount(fileHandler& DB, AccountManager& AllUsers){
        DB.readToken();
        std::string account_type =  DB.getToken();
        DB.readToken();
        std::string id = DB.getToken();
        DB.readToken();
        std::string pw = DB.getToken();

        if(!account_type.compare("ADMIN")){
            return new Account(ADMIN,id,pw);
        }else{
            return new Account(USER,id,pw);
        }
    }
    void formatData(fileHandler& DB, AccountManager& AllUsers){
        while (DB.readLine()){
           Account* a = createAccount(DB, AllUsers);
           AllUsers.addAccount(a);
        }   
    }
    void modifyData(fileHandler& DB, AccountManager& AllUsers, Account& newAccount){
        fileHandler temp("dataset/temp.txt");
        Account* target = AllUsers.find(newAccount.getId(), newAccount.getPw());
        target -> setId(newAccount.getId());
        target -> setPw(newAccount.getPw());
        DB.moveFront();
        while (DB.readLine()){
            DB.readToken();
            std::string type = DB.getToken();
            DB.readToken();
            std::string id = DB.getToken();
            DB.readToken();
            std::string pw = DB.getToken();

            if(!id.compare(newAccount.getId())){
                temp.writeToken(type + "\t" + id + "\t" +pw + "\n");
            }else{
                temp.writeToken(DB.getLine());
            }
        }
        DB.clear();
        temp.moveFront();
        while (temp.readLine()){
            DB.writeToken(temp.getLine());
        }
        temp.clear();
    }
};

class orderHistoryMaker{
private:
public:
    orderHistoryMaker(){}
    Account* createAccount(fileHandler& DB, AccountManager& AllUsers){
        DB.readToken();
        std::string account_type =  DB.getToken();
        DB.readToken();
        std::string id = DB.getToken();
        DB.readToken();
        std::string pw = DB.getToken();

        if(!account_type.compare("ADMIN")){
            return new Account(ADMIN,id,pw);
        }else{
            return new Account(USER,id,pw);
        }
    }
    void formatData(fileHandler& DB, AccountManager& AllUsers){
        while (DB.readLine()){
           Account* a = createAccount(DB, AllUsers);
           AllUsers.addAccount(a);
        }   
    }
};


//class implementation parts.
//main menus -> all functions below can be overwrite an action function

//if the function return true, it means need new page else, remove current page.
//every function keep its page by using cin.
//if user gives any input w/o \n, -> terminate current page and return false.

class signInPage{
private:
    fileHandler& DB;
    AccountManager& userlist;
    accountMaker& accountInfoMaker;
public:
    signInPage(fileHandler& DB,AccountManager& userlist,accountMaker& accountInfoMaker):DB(DB),userlist(userlist), accountInfoMaker(accountInfoMaker){}
    bool action(){
        accountInfoMaker.formatData(DB, userlist);
        std::string id,pw;

        std::cout << "This is Sign In page...." << std::endl << std::endl;
        std::cout << "Input the ID.." << std::endl;
        std::cin >> id;
        std::cout << "Input the PW.." << std::endl;
        std::cin >> pw;

        if(userlist.login(id, pw)){
            std::cout << "Hello " << id << std::endl;
            return true;
        }else{
            std::cout << "not found... try again."<< std::endl;
        }
        std::cout << "go back with press enter" << std::endl;

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class signUpPage{
private:
    fileHandler& DB;
    AccountManager& userlist;
    accountMaker& accountInfoMaker;
public:
//여기에서 생성.
    signUpPage(fileHandler& DB,AccountManager& userlist,accountMaker& accountInfoMaker):DB(DB),userlist(userlist), accountInfoMaker(accountInfoMaker){}
    bool action(){
        std::cout << "This is Sign Up page...." << std::endl;
        accountInfoMaker.formatData(DB, userlist);
        std::string id,pw;
        std::string state = "1";
        AccountType accountType;
        int buffer;
        //여기 잘못 입력하면 에러 있음. -> sdf
        do{
            try{
                std::cout << "Type of Account(0 to exit, 1 for Admin, 2 for User): ";
                std::cin >> buffer;
                if(buffer == 0){
                    break;
                }else if( buffer == 1){
                    accountType = ADMIN;
                }else if( buffer == 2 ){
                    accountType = USER;
                }else{
                    throw std::invalid_argument("wrong intput.");
                }
	            std::cout << "Set your ID: ";
	            std::cin >> id;
	            std::cout << "Set your PW: ";
	            std::cin >> pw;

                userlist.addAccount(new Account(accountType, id, pw));

            }catch(const std::exception& e){
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

class searchPage{
private:
    InfoMaker& storeInfoMaker;
    storeVector& storeList;
    fileHandler& DB;
public:
    searchPage(fileHandler& DB, storeVector& storeList, InfoMaker& storeInfoMaker): DB(DB),storeList(storeList),storeInfoMaker(storeInfoMaker){}
    bool action(){

        //dataset part....
        storeInfoMaker.formatData(DB, storeList);
        std::cout << "This is Search page...." << std::endl;
	
        std::string inputMenu;
        std::cout << "Search(0 to exit): 1-storeName, 2-menuName";
        std::cin >> inputMenu;

        if(!inputMenu.compare("0")){    //exit.
            return false;
        }else if(!inputMenu.compare("1")){
            std::cout << "Store? :";
            std::cin >> inputMenu;
            int i=1;
            for(menuVector* storeItem : storeList.findStore2Name(inputMenu)){
                std::cout << i << "번째";
                i++;
                std::cout << "[" << storeItem -> getName() << "]\t 추천수:" << storeItem -> getReview() << std::endl;

                for(menu* menuItem : storeItem -> allMenu()){

                    std::cout << "- ";
                    //한글이 cout에서 3배 크기를 차지한다. -> 초,중,종성으로 따로 저장하니.
                    stringK menuItemName(menuItem -> getMenuName());
                    int formatInterval = 70;
                    std::cout << std::left << menuItemName.getStr() << std::right << std::setw(formatInterval -menuItemName.size()) << "|";            
                    std::cout << std::right << std::setw(10) << menuItem -> getMenuPrice();

                    if(menuItem -> getMenuName().find(inputMenu) != std::string::npos){
                        std::cout << "  *" << std::endl;
                    }else{
                        std::cout << std::endl;
                    }
                }
                std::cout << std::endl;
            }
            
        }else if(!inputMenu.compare("2")){
            std::cout << "Menu? :";
            std::cin >> inputMenu;

            std::cout << "Sort? 0: default, 1: price, 2: review : ";
            int orderinput;
            std::cin >> orderinput;
    
            int i = 0;
            for(menuVector* storeItem : storeList.findStore2Menu(inputMenu, orderinput)){
                std::cout << i << "번째";
                i++;
                std::cout << "[" << storeItem -> getName() << "]\t 추천수:" << storeItem -> getReview() << std::endl;

                for(menu* menuItem : storeItem -> allMenu()){

                    std::cout << "- ";
                    //한글이 cout에서 3배 크기를 차지한다. -> 초,중,종성으로 따로 저장하니.
                    stringK menuItemName(menuItem -> getMenuName());
                    int formatInterval = 70;
                    std::cout << std::left << menuItemName.getStr() << std::right << std::setw(formatInterval -menuItemName.size()) << "|";            
                    std::cout << std::right << std::setw(10) << menuItem -> getMenuPrice();

                    if(menuItem -> getMenuName().find(inputMenu) != std::string::npos){
                        std::cout << "  *" << std::endl;
                    }else{
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

class categoriesPage{
private:
public:
    bool action(){
        std::string category;
        std::cout << "This is category page...." << std::endl;
        std::cout << "Type category what you want(0 to exit, 1 for Hansik, 2 for Chinese..): ";
        std::cin >> category;

        if(1){
            return true;
        }

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

// 아래 2개는 이건 txt파일이랑 연동되어야해서 ..
class orderHistoryPage{
private:
public:
    bool action(){
        std::cout << "This is order history page...." << std::endl;
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class favoritesPage{
private:
public:
    bool action(){
        std::cout << "This is favorite page...." << std::endl;
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class accountInfoPage{
private:
    accountMaker& accountMaker1;
    fileHandler& DB;
    AccountManager& Allusers;
public:
    accountInfoPage(fileHandler& DB, AccountManager& Allusers, accountMaker& accountMaker1):DB(DB), Allusers(Allusers),accountMaker1(accountMaker1){}
    bool action(){
        std::cout << "This is account info page...." << std::endl;

        accountMaker1.formatData(DB, Allusers);
    
        if( Allusers.getAccount() -> getAccountType() == ADMIN ){
            std::cout << "Account Type: Admin" << std::endl; 
        }else{
            std::cout << "Account Type: User" << std::endl;
        }

        std::cout << "ID: " << Allusers.getAccount() -> getId() << std::endl;
        std::cout << "PW: " << Allusers.getAccount() -> getPw() << std::endl << std::endl;

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class modifyRestaurant{
private:
    fileHandler& DB;
    storeVector& storelist;
    InfoMaker& storeInfoMaker;
public:
    modifyRestaurant(fileHandler& DB, storeVector& storelist, InfoMaker& storeInfoMaker): DB(DB), storelist(storelist), storeInfoMaker(storeInfoMaker){}
    bool action(){
        std::cout << "This is restaurant modify page...." << std::endl;
        std::string input;

        std::vector<menuVector*> targetStoreList;
        menuVector* targetStore;
        menu* targetMenu;

        std::cout << "Input restaurant's name to find(0 to exit): ";
        std::cin >> input;

        if(input.compare("0")){
            std::string restaurantName = input;
            targetStoreList = storelist.findStore2Name(restaurantName);

            if(targetStoreList.empty()){ //store does not exist.
                std::cout << "no store..." << std::endl;
                std::cin.get();
                std::cin.ignore();
                return false;
            }else{
                std::cout << "Choose the restaurant: " << std::endl;

                int cnt = 0;
                for(menuVector* _store: targetStoreList){
                    std::cout << cnt + 1 << " " << _store -> getName() << std::endl;
                }
                std::cin >> cnt;
                targetStore = targetStoreList[cnt];

                std::cout << "Input restaurant's name to modify(0 to exit): ";
                std::cin >> restaurantName;
                std::string menuName;
                std::string newPrice;

                if(restaurantName.compare("0")){
                    size_t i=1;

                    //targetStore -> allMenu();
                    /*
                    for(menu* menuItem : targetStore -> allMenu()){
                        std::cout << i << ". " << menuItem -> getMenuName() << " " << menuItem -> getMenuPrice() << std::endl;
                        i++;
                    }
                    */
                    std::cout << "Number for a menu, 0 to exit : ";
                    int count = 1;
                    
                    std::cin >> input;
                    if(input.compare("0")){
                        targetMenu = (targetStore -> allMenu())[i-1];
                        std::string newMenuName, newMenuPrice;
                        std::cout << "Name : ";
                        std::cin >> newMenuName;
                        targetMenu -> setMenuName(newMenuName);
                        std::cout << "Price(format ex. 18,000) :";
                        std::cin >> newMenuPrice;
                        targetMenu -> setMenuPrice(newMenuPrice);
                        storeInfoMaker.modifyData(DB, *targetStore, *targetMenu);
                    }else{
                        return false;
                    }
                
                }else{
                    return false;
                }

            }
        }else{
            return false;
        }






        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};


//categories page menus

class hansik{
private:
public:
    bool action(){
        //std::string input;
        std::cout << "This is hansik page...." << std::endl;
        std::cout << "press any to go back";
    
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class chinese{
private:
public:
    bool action(){
        //std::string input;
        std::cout << "This is chinese page...." << std::endl;
        std::cout << "press any to go back";
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

int main(){
    //make infoMaker
    InfoMaker storeInfoMaker;
    accountMaker accountInfoMaker;

    //read DB and make fileHanlder to control it. 
    std::string DB_path = "./dataset/DB_stores.txt";
    fileHandler DB(DB_path);
    std::string DB_Account_path = "./dataset/DB_accounts.txt";
    fileHandler DB_Account(DB_Account_path);
    std::string DB_OrderHistory_path = "./dataset/DB_history.txt";
    fileHandler DB_OrderHistory(DB_OrderHistory_path);
    
    //storage
    storeVector storeList;

    // make Admin and user account.
    AccountManager AllUsers;
    //Account Admin(ADMIN,"admin", "admin");
    //Account User1(USER, "user", "user");
    //AllUsers.addAccount(&Admin);
    //AllUsers.addAccount(&User1);
    //AllUsers.removeAccount(&User1); -> implementation needed.

    //Structure of Main Page
    MenuPage mainPage("Main Page");
    MenuItem SignInPage("Sign In", new signInPage(DB_Account, AllUsers, accountInfoMaker));
    MenuItem SignUpPage("Sign Up", new signUpPage(DB_Account, AllUsers, accountInfoMaker));
    mainPage.addMenu(&SignInPage);
    mainPage.addMenu(&SignUpPage);

    //Sturucture of User Main Page.
    MenuPage userMainPage("User Main Page");
    MenuItem SearchPage("searchPage",new searchPage(DB, storeList, storeInfoMaker));
    MenuItem CategoryPage("CategoriesPage",new categoriesPage);
    MenuItem OrderHistoryPage("OrderHistoryPage",new orderHistoryPage);
    MenuItem FavoritesPage("FavoritesPage",new favoritesPage);
    MenuItem AccountInfoPage("AccountInfoPage",new accountInfoPage(DB_Account, AllUsers, accountInfoMaker));
    MenuItem ModifyRestaurant("Modify Restaurant",new modifyRestaurant(DB, storeList, storeInfoMaker));
    userMainPage.addMenu(&SearchPage);
    userMainPage.addMenu(&CategoryPage);
    userMainPage.addMenu(&OrderHistoryPage);
    userMainPage.addMenu(&FavoritesPage);
    userMainPage.addMenu(&AccountInfoPage);
    userMainPage.addMenu(&ModifyRestaurant);

    //Structure of Categories Page.
    MenuPage categoriesMenuPage("Categories Menu Page");
    MenuItem HansikPage("hansikPage",new hansik);
    categoriesMenuPage.addMenu(&HansikPage);
    MenuItem chinesePage("chinesePage",new chinese);
    categoriesMenuPage.addMenu(&chinesePage);
    
    //Structure of Order History Page
    
    //pageControl such like browser
    pageControler browser(AllUsers);
    browser.addPageTemplate(&mainPage);
    browser.addPageTemplate(&userMainPage);
    browser.addPageTemplate(&categoriesMenuPage);

    //execute browser
    browser.addPage(&mainPage);
    browser.run();

    return 0;
}
 



//admin -> event input, //아니면 할인율 정보 -> 한일율 정렬이나 추천.


//implementation 이랑 헤더랑 빼기
//site 의 구조에서 history를 분리해서 메멘토로 뺴자.
//추천 시스템인데 뭔가  interesting을 줄만한게 있냐..?
//제안점. 1. global scope에 있는 함수들을 각 객체로 묶어라. 2. sort함수를 extend가능하게 만들어라.(별도함수 분리.) 3. store 역시 extend가능하게 abstract 추가해서 묶자.(메뉴랑, 가게) 그쪽이 너무 tight 하다.