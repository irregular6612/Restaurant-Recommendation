#include <vector>
#include <functional>
#include "account.hpp"

//command가 굳이 필요한가...?
class Command {
private:
public:
    virtual bool execute() = 0;
    virtual std::string getName() const = 0;
};

template <typename T>//템플릿에서 추상으로 
class FunctionalPage : public Command { //conrete command
private:
    std::string pageName;
    T* action;
public:
    FunctionalPage(const std::string& name, T* action) : pageName(name), action(action) {}
    bool execute() override {
        return action->action();
    };

    std::string getName() const override {
        return pageName;
    }
};

class PrintOnlyPage {
private:
    std::vector<Command*> menuList;
    std::string pageName;
public:
    PrintOnlyPage(std::string name) :pageName(name) {};
    void addMenu(Command* item) { menuList.push_back(item);  }
    bool show() {
        int selection = 0;
        do {
            system("clear");
            std::cout << "Menu:" << pageName << std::endl << std::endl;
            for (int i = 0; i < menuList.size(); i++) {
                std::cout << i + 1 << ". " << menuList[i]->getName() << std::endl;
            }
            std::cout << "0. exit" << std::endl << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> selection;
            if (selection > 0 && menuList.size()) {
                system("clear");
                if (menuList[selection - 1]->execute()) {
                    return true;
                }
            }
        } while (selection != 0);
        return false;
    }
};

class PageController {
private:
    enum PageState { MAINPAGE, USERMAIN, CATEGORIESPAGE };

    PageState pageState;
    std::vector<PrintOnlyPage*> pageHistory;
    std::vector<PrintOnlyPage*> pageTemplate;
    AccountStorage& accountStorage;
    bool runState;
public:
    PageController(AccountStorage& accountStorage) : accountStorage(accountStorage), runState(true), pageState(MAINPAGE) {}
    void addPage(PrintOnlyPage* page) {
        pageHistory.push_back(page);
    }
    void restorePage() {
        pageHistory.pop_back();
        //printPage();
    }
    bool isLoggedIn() {
        if (accountStorage.getAccount() != nullptr) { return true; }
        else { return false; }
    }
    void addPageTemplate(PrintOnlyPage* page) {
        pageTemplate.push_back(page);
    }
    bool printPage() {
        return (pageHistory.back()->show());
    }
    void run() {
        while (pageHistory.size()) {
            if (printPage() == false) {
                restorePage();
                //여기서 enum 때문에...
                pageState = static_cast<PageState>(static_cast<int>(pageState) - 1);
            }
            else {
                addPage(pageTemplate[static_cast<int>(pageState) + 1]);
                pageState = static_cast<PageState>(static_cast<int>(pageState) + 1);
            }
        }
    }
};