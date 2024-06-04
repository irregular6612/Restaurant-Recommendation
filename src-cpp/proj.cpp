#include "fileHandler.hpp"
#include "storeVector.hpp"
#include "stringK.hpp"

int main(int argc, char* argv[]){

    //dataset part....
    
    std::string DB_path= "./dataset/DB_stores.txt";
    //std::string Account_path = "./dataset/DB_accounts.txt";
    //std::string History_path = "./dataset/DB_history.txt";

    fileHandler DB(DB_path);
    storeVector storeList;

    while (DB.readLine()){

        if(DB.getLine() == ""){ //if there is a blank line -> new storeInfo starting next line
            DB.readLine();  //thus, read one more line.
        }
        
        DB.readToken(); //read the storeName
        store* s = new store(DB.getToken());    //make a store pointer with its name
        DB.readToken();//read the category
        s -> setCatergory(DB.getToken());

        DB.readLine();  //read new line
        DB.readToken(); //read Telinfo
        s -> setTel(DB.getToken());  //set tel.
        DB.readToken(); //read reviewCount
        s -> setReview(DB.getToken());  //set reviewCount.

        DB.readLine();  //read the menuInfo line.. ex -> 고등어조림 15,000 | 갈치조림 18,000 | 쌈밥 10,000 | 생선 모둠구이 15,000

        while(DB.readToken('|')){   // delimeter is |
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

            menu* m1 = new menu(menuName, menuPrice);
            s->addMenu(m1);
        }
        if(storeList.findStore2Name( s -> getName()) == nullptr){
            storeList.insertStore(s);
        }
    }


    std::string inputMenu;
    std::cout << "Menu? :";
    std::cin >> inputMenu;
    //edit for debugging
    //inputMenu = "제육볶음";
    int i=1;

    ///*
    // 0-> default, 1 -> price, 2 -> stars
    std::cout << "Sort? 0: default, 1: price, 2: review : ";
    int orderinput;
    std::cin >> orderinput;

    for(store* storeItem : storeList.findStore2Menu(inputMenu, orderinput)){
        
        std::cout << i << "번째";
        i++;
        std::cout << "[" << storeItem -> getName() << "]\t 추천수:" << storeItem -> getReview() << std::endl;

        for(menu* menuItem : storeItem ->allMenu()){

            std::cout << "- ";
            //한글이 cout에서 3배 크기를 차지한다. -> 초,중,종성으로 따로 저장하니.
            stringK menuItemName(menuItem ->getName());
            int formatInterval = 70;
            std::cout << std::left << menuItemName.getStr() << std::right << std::setw(formatInterval -menuItemName.size()) << "|";            
            std::cout << std::right << std::setw(10) << menuItem -> getPrice();

            if(menuItem -> getName().find(inputMenu) != std::string::npos){
                std::cout << "  *" << std::endl;
            }else{
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
    //*/
    return 0;
}


//정렬 기준: 가격, 별점, default 순서
//가격이 없는 거는 지우자.