#ifndef BROWSER_H
#define BROWSER_H
#include "account.hpp"


//this is for command pattern(design pattern)
//interface
class Command {
private:
public:
    virtual bool execute() = 0;
    virtual std::string getName() const = 0;
};

//abstract of receiver
class ActionPage {
public:
    virtual bool action() = 0;
};

//conrete command
class FunctionalPage : public Command { 
private:
    std::string pageName;
    ActionPage* action;
public:
    FunctionalPage(const std::string& name, ActionPage* action);
    bool execute() override;

    std::string getName() const override;
};

//concrete recevier classes is in the pages.hpp


//memento pattern to care browser history.

//memento
class PrintOnlyPageState {
private:
    std::vector<Command*> menuList;
    std::string pageName;
public:
    PrintOnlyPageState(std::vector<Command*> menuList, std::string pageName);
    std::vector<Command*> getMenuList();
    std::string getPageName();
};
//originator and also invoker class
class PrintOnlyPage {
private:
    std::vector<Command*> menuList;
    std::string pageName;
public:
    PrintOnlyPage(std::string name);
    void addMenu(Command* item);
    bool show();
    PrintOnlyPageState getPrintOnlyPageState();
    void setPageState(PrintOnlyPageState printOnlyPageState);
};
//caretaker
class PrintOnlyPageHistory {
private:
    std::vector<PrintOnlyPageState> historyVector;
public:
    void push(PrintOnlyPageState printOnlyPageState);
    PrintOnlyPageState pop();
    int size();
};

//browser class.

class PageController {
private:
    enum PageState { MAINPAGE, USERMAIN, CATEGORIESPAGE };

    PageState pageState;
    PrintOnlyPage* printOnlyPage;
    PrintOnlyPageHistory printOnlyPageHistory;
    //std::vector<PrintOnlyPage*> pageHistory;
    std::vector<PrintOnlyPage*> pageTemplate;
    AccountStorage& accountStorage;
    bool runState;
public:
    PageController(AccountStorage& accountStorage);
    void addPage(PrintOnlyPage* page);
    void restorePage();
    bool isLoggedIn();
    void addPageTemplate(PrintOnlyPage* page);
    PrintOnlyPage* getPageTemplate(PageState);
    bool printPage();
    void run();
};

#endif