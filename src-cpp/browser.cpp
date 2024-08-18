
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <exception>
#include "../header-cpp/browser.hpp"

FunctionalPage::FunctionalPage(const std::string& name, ActionPage* action) : pageName(name), action(action) {}

bool FunctionalPage::execute() {
    std::cout << "[ " << pageName << " ]" << std::endl << std::endl;
    return action->action();
}

std::string FunctionalPage::getName() const {
    return pageName;
}

PrintOnlyPage::PrintOnlyPage(std::string name) :pageName(name) {}

void PrintOnlyPage::addMenu(Command* item) { menuList.push_back(item); }

bool PrintOnlyPage::show() {
    int selection = 0;
    do {
        //consider type of os
        #ifdef _WIN32
            system("cls");
        #endif

        #ifdef __APPLE__
            system("clear");
        #endif

        std::cout << "Menu: " << pageName << std::endl << std::endl;
        for (int i = 0; i < menuList.size(); i++) {
            std::cout << i + 1 << ". " << menuList[i]->getName() << std::endl;
        }
        std::cout << "0. exit" << std::endl << std::endl;
        std::cout << "Choose an option: ";
        std::cin >> selection;
        if (selection > 0 && menuList.size()) {

            #ifdef _WIN32
                system("cls");
            #endif

            #ifdef __APPLE__
                system("clear");
            #endif

            if (menuList[selection - 1]->execute()) {
                return true;
            }
        }
    } while (selection != 0);
    return false;
}

PrintOnlyPageState PrintOnlyPage::getPrintOnlyPageState() {
    PrintOnlyPageState returnState(this->menuList, this->pageName);
    return returnState;
}

void PrintOnlyPage::setPageState(PrintOnlyPageState printOnlyPageState) {
    menuList = printOnlyPageState.getMenuList();
    pageName = printOnlyPageState.getPageName();
}

PrintOnlyPageState::PrintOnlyPageState(std::vector<Command*> menuList, std::string pageName) :menuList(menuList), pageName(pageName) {}

std::vector<Command*> PrintOnlyPageState::getMenuList() {
    return menuList;
}

std::string PrintOnlyPageState::getPageName() {
    return pageName;
}

void PrintOnlyPageHistory::push(PrintOnlyPageState printOnlyPageState) {
    historyVector.push_back(printOnlyPageState);
}

PrintOnlyPageState PrintOnlyPageHistory::pop() {
    historyVector.pop_back();
    if (!historyVector.size())
        throw std::length_error("Program end");
    return historyVector.back();
    
}

int PrintOnlyPageHistory::size() {
    return historyVector.size();
}

PageController::PageController(AccountStorage& accountStorage) : accountStorage(accountStorage), runState(true), pageState(MAINPAGE) {}

void PageController::addPage(PrintOnlyPage* page) {
    //fix the bug.. which about the memento
    //printOnlyPage = page; -> modify the template.
    std::cout << "new template: " << page ->getPrintOnlyPageState().getPageName() << std::endl;
    printOnlyPage = new PrintOnlyPage(page -> getPrintOnlyPageState().getPageName());
    printOnlyPage -> setPageState(page -> getPrintOnlyPageState());
    
    printOnlyPageHistory.push(page->getPrintOnlyPageState());
}

void PageController::restorePage() {
    if (printOnlyPageHistory.size())
        printOnlyPage->setPageState(printOnlyPageHistory.pop());
}

bool PageController::isLoggedIn() {
    if (accountStorage.getAccount() != nullptr) { return true; }
    else { return false; }
}

void PageController::addPageTemplate(PrintOnlyPage* page) {
    pageTemplate.push_back(page);
}

bool PageController::printPage() {
    return printOnlyPage->show();
}

PrintOnlyPage* PageController::getPageTemplate(PageState state){
    PrintOnlyPage* rerturnvalue = pageTemplate[state];
    return rerturnvalue;
}

void PageController::run() {
    try {
        while (printOnlyPageHistory.size()) {
            if (printPage() == false) {
                restorePage();
                pageState = static_cast<PageState>(static_cast<int>(pageState) - 1);
            }
            else {
                pageState = static_cast<PageState>(static_cast<int>(pageState) + 1);
                addPage(pageTemplate[pageState]);
            }
        }
    }
    catch (std::length_error) {}
}