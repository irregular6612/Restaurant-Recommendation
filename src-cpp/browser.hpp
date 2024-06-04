#include<vector>
#include<functional>
#include"Account.hpp"

//command가 굳이 필요한가...?
class Command{
private:
public:
    virtual bool execute(AccountManager*) = 0;
    virtual std::string getName() const = 0;
};

template <typename T>
class MenuItem: public Command{
private:
    std::string name;
    T* action;
public:
    MenuItem(const std::string& name, T* action): name(name), action(action){}
    bool execute(AccountManager* userlist) override{
        return action -> action(userlist);
    };

    std::string getName() const override{
        return name;
    }
};

class MenuPage{
private:
    std::vector<Command*> menulist;
    std::string name;
public:
    MenuPage(std::string name):name(name){};
    void addMenu(Command* item){ menulist.push_back(item); }
    bool show(AccountManager* userlist){
        int selection = 0;
        do{
            system("clear");
            std::cout << "Menu:" << name << std::endl << std::endl;
            for(int i=0; i< menulist.size();i++){
                std::cout << i + 1 << ". " << menulist[i] -> getName() << std::endl;   
            }
            std::cout << "0. exit" << std::endl << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> selection;
            if(selection > 0 && menulist.size()){
                system("clear");
                if(menulist[selection -1] -> execute(userlist)){
                    return true;
                }
            }
        }while(selection != 0);
        return false;
    }
};

class pageControler{
private:
    enum class pageState{ MAINPAGE, USERMAIN, CATEGORIESPAGE };

    pageState curPage;
    std::vector<MenuPage*> pagehistory;
    std::vector<MenuPage*> pageTemplate;
    AccountManager* userlist;
    bool runstate;
public:
    pageControler(AccountManager* userlist):userlist(userlist), runstate(true), curPage(pageState::MAINPAGE){}
    void addPage(MenuPage* page){
        pagehistory.push_back(page);
    }
    void restorePage(){
        pagehistory.pop_back();
        //printPage();
    }
    bool isLogedIn(){
        if(userlist->getAccount() != nullptr){ return true; }
        else{ return false; }
    }
    void addPageTemplate(MenuPage* page){
        pageTemplate.push_back(page);
    }
    bool printPage(){
        return (pagehistory.back() -> show(userlist));
    }
    void run(){
        while (pagehistory.size()){
            if(printPage() == false){
                restorePage();
                //여기서 enum 때문에...
                curPage = static_cast<pageState>(static_cast<int>(curPage) - 1);
            }else{
                addPage(pageTemplate[static_cast<int>(curPage) + 1]);
                curPage = static_cast<pageState>(static_cast<int>(curPage) + 1);
            }
        }
    }
};