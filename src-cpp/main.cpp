#include <iostream>
#include <ctime>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <algorithm>
#include <string>

#include "../header-cpp/pages.hpp"

//g++ -std=c++20 -stdlib=libc++ -o $fileName $fileNameWithoutExt
//g++ -std=c++20 -stdlib=libc++ -o main main.cpp account.cpp browser.cpp filehandler.cpp modifier.cpp pages.cpp storeVector.cpp stringK.cpp

int main() {

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
    AccountStorage accountStorage;

    //make information modifier
    StoreModifier storeModifier;
    AccountModifier accountModifier;

    //Structure of Main Page
    PrintOnlyPage mainPage("Main Page");
    FunctionalPage signInPage("Sign In", new SignInPage(dbAccount, accountStorage, accountModifier));
    FunctionalPage signUpPage("Sign Up", new SignUpPage(dbAccount, accountStorage, accountModifier));
    mainPage.addMenu(&signInPage);
    mainPage.addMenu(&signUpPage);

    //Sturucture of User Main Page.
    PrintOnlyPage userMainPage("User Main Page");
    FunctionalPage searchPage("Search Page", new SearchPage(db, storeVector, storeModifier));
    FunctionalPage categoriesPage("Categories Page", new CategoriesPage);
    FunctionalPage orderHistoryPage("Order History Page", new OrderHistoryPage(dbOrderHistory, accountStorage));
    FunctionalPage favoritesPage("Favorites Page", new FavoritesPage(dbFavorites, accountStorage));
    FunctionalPage accountInfoPage("AccountInfo Page", new AccountInfoPage(dbAccount, accountStorage, accountModifier));
    FunctionalPage modifyRestaurantPage("Modify Restaurant Page", new ModifyRestaurantPage(db, storeVector, storeModifier, accountStorage));

    //easily extend new pages.
    FunctionalPage discountInfoPage("Discount InfoPage", new DiscountInfoPage(storeVector));

    userMainPage.addMenu(&searchPage);
    userMainPage.addMenu(&categoriesPage);
    userMainPage.addMenu(&orderHistoryPage);
    userMainPage.addMenu(&favoritesPage);
    userMainPage.addMenu(&accountInfoPage);
    userMainPage.addMenu(&modifyRestaurantPage);
    userMainPage.addMenu(&discountInfoPage);

    //Structure of Categories Page.
    PrintOnlyPage categoryStoresPage("Category's Stores Page");
    FunctionalPage koreanPage("Korean", new CategoriesRestaurantsPage(storeVector, "Korean"));
    categoryStoresPage.addMenu(&koreanPage);
    FunctionalPage chinesePage("Chinese", new CategoriesRestaurantsPage(storeVector, "Chinese"));
    categoryStoresPage.addMenu(&chinesePage);
    FunctionalPage japanesePage("Japanese", new CategoriesRestaurantsPage(storeVector, "Japanese"));
    categoryStoresPage.addMenu(&japanesePage);
    FunctionalPage westernPage("Western", new CategoriesRestaurantsPage(storeVector, "Western"));
    categoryStoresPage.addMenu(&westernPage);
    FunctionalPage fastfoodPage("Fastfood", new CategoriesRestaurantsPage(storeVector, "Fastfood"));
    categoryStoresPage.addMenu(&fastfoodPage);
    FunctionalPage boonsikPage("Boonsik", new CategoriesRestaurantsPage(storeVector, "Boonsik"));
    categoryStoresPage.addMenu(&boonsikPage);
    FunctionalPage dessertPage("Dessert", new CategoriesRestaurantsPage(storeVector, "Dessert"));
    categoryStoresPage.addMenu(&dessertPage);
    FunctionalPage etcPage("etc", new CategoriesRestaurantsPage(storeVector, "etc"));
    categoryStoresPage.addMenu(&etcPage);

    //PageController such like browser
    PageController browser(accountStorage);
    browser.addPageTemplate(&mainPage);
    browser.addPageTemplate(&userMainPage);
    browser.addPageTemplate(&categoryStoresPage);

    //execute browser
    storeModifier.formatData(db, storeVector);  //load data on memory.
    browser.addPage(&mainPage);
    browser.run();

    return 0;
}

/*
Descriptions in perspective of OOP class.

[Design pattern]
1. command pattern in menupage(PrintOnlyPage) - easy to extend menus.
2. memento pattern in browser(PrintOnlyPage, PrintOnlyHistory, PrintOnlyState) - single responsibility.

[Polymorphism]
1. ActionPage(abstract) : receiver object of command pattern

[Friend function or class]
1. we do not used. to avoid open-closed principle(to modify)

[Deployment and File management]
1. separate .hpp and .cpp





*/