#include<iostream>
#include<memory>
#include"browser.hpp"
#include"fileHandler.hpp"
#include "storeVector.hpp"
#include "stringK.hpp"


// data part...
class menuCreator{
private:
    fileHandler& DB;
    storeVector& storelist;
public:
    menuCreator(fileHandler& DB, storeVector& storeList): DB(DB), storelist(storelist){}   
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
            if(storeList.findStore2Name( s -> getName()) == nullptr){
                storeList.insertStore(s);
            }
        }
    }
};

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
        if(storeList.findStore2Name( s -> getName()) == nullptr){
            storeList.insertStore(s);
        }
    }
}


//function implementation parts.
//main menus -> all functions below can be overwrite an action function

//if the function return true, it means need new page else, remove current page.
//every function keep its page by using cin.
//if user gives any input w/o \n, -> terminate current page and return false.

class signInPage{
private:
public:
    bool action(AccountManager* userlist){
        std::string id,pw;

        std::cout << "This is Sign In page...." << std::endl << std::endl;
        std::cout << "Input the ID.." << std::endl;
        std::cin >> id;
        std::cout << "Input the PW.." << std::endl;
        std::cin >> pw;

        if(userlist -> login(id, pw)){
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
public:
    bool action(AccountManager* userlist){
        std::cout << "This is Sign Up page...." << std::endl;
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

                userlist->addAccount(new Account(accountType, id, pw));

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
public:
    bool action(AccountManager* userlist){

        //dataset part....
        std::string DB_path= "./dataset/DB_stores.txt";
        //std::string Account_path = "./dataset/DB_accounts.txt";
        //std::string History_path = "./dataset/DB_history.txt";
        fileHandler DB(DB_path);
        storeVector storeList;

        formatData(DB, storeList);
        std::cout << "This is Search page...." << std::endl;
	
        std::string inputMenu;
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

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class categoriesPage{
private:
public:
    bool action(AccountManager* userlist){
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
    bool action(AccountManager* userlist){
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
    bool action(AccountManager* userlist){
        std::cout << "This is favorite page...." << std::endl;
        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class accountInfoPage{
private:
public:
    bool action(AccountManager* userlist){
        std::cout << "This is account info page...." << std::endl;
    
        if( userlist ->getAccount() -> getAccountType() == ADMIN ){
            std::cout << "Account Type: Admin" << std::endl; 
        }else{
            std::cout << "Account Type: User" << std::endl;
        }

        std::cout << "ID: " << userlist -> getAccount() -> getId() << std::endl;
        std::cout << "PW: " << userlist -> getAccount() -> getPw() << std::endl << std::endl;

        //for delay
        std::cin.get();
        std::cin.ignore();
        return false;
    }
};

class modifyRestaurant{
private:
public:
    bool action(AccountManager* userlist){
        std::cout << "This is restaurant modify page...." << std::endl;
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
    bool action(AccountManager* userlist){
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
    bool action(AccountManager* userlist){
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

    // make Admin and user account.
    AccountManager AllUsers;
    Account Admin(ADMIN,"admin", "admin");
    Account User1(USER, "user", "user");
    AllUsers.addAccount(&Admin);
    AllUsers.addAccount(&User1);
    //AllUsers.removeAccount(&User1); -> implementation needed.

    //Structure of Main Page
    MenuPage mainPage("Main Page");
    MenuItem SignInPage("Sign In", new signInPage);
    MenuItem SignUpPage("Sign Up", new signUpPage);
    mainPage.addMenu(&SignInPage);
    mainPage.addMenu(&SignUpPage);

    //Sturucture of User Main Page.
    MenuPage userMainPage("User Main Page");
    MenuItem SearchPage("searchPage",new searchPage);
    MenuItem CategoryPage("CategoriesPage",new categoriesPage);
    MenuItem OrderHistoryPage("OrderHistoryPage",new orderHistoryPage);
    MenuItem FavoritesPage("FavoritesPage",new favoritesPage);
    MenuItem AccountInfoPage("AccountInfoPage",new accountInfoPage);
    MenuItem ModifyRestaurant("Modify Restaurant",new modifyRestaurant);
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
    pageControler browser(&AllUsers);
    browser.addPageTemplate(&mainPage);
    browser.addPageTemplate(&userMainPage);
    browser.addPageTemplate(&categoriesMenuPage);

    //execute browser
    browser.addPage(&mainPage);
    browser.run();

    return 0;
}
 

//site 의 구조에서 history를 분리해서 메멘토로 뺴자.
//추천 시스템인데 뭔가  interesting을 줄만한게 있냐..?
//제안점. 1. global scope에 있는 함수들을 각 객체로 묶어라. 2. sort함수를 extend가능하게 만들어라.(별도함수 분리.) 3. store 역시 extend가능하게 abstract 추가해서 묶자.(메뉴랑, 가게) 그쪽이 너무 tight 하다.