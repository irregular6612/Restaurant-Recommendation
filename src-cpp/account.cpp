#include <iostream>
#include <string>
#include "../header-cpp/account.hpp"

Account::Account(AccountType accountType, std::string id, std::string pw) :accountType(accountType), id(id), pw(pw) {}

void Account::setId(std::string id) { this->id = id; }

std::string Account::getId() const { return id; }

void Account::setPw(std::string pw) { this->pw = pw; }

std::string Account::getPw() const { return pw; }

AccountType Account::getAccountType() const { return accountType; }

AccountStorage::AccountStorage() : account(nullptr) {}

void AccountStorage::addAccount(Account* account) { accountList.push_back(account); }

void AccountStorage::removeAccount(Account* account) {
    for (std::vector<Account*>::iterator it = accountList.begin(); it != accountList.end(); it++) {
        size();
        accountList.erase(accountList.begin());
    }
}

int AccountStorage::size() { std::cout << "Account : " << accountList.size() << std::endl; return accountList.size(); }

Account* AccountStorage::find(std::string id, std::string pw) {
    for (Account* account : accountList) {
        if (!(account->getId().compare(id)) && !(account->getPw().compare(pw))) {
            return account;
        }
    }
    return nullptr;
}

bool AccountStorage::login(std::string id, std::string pw) {
    Account* targetAccount = find(id, pw);
    if (targetAccount != nullptr) {
        account = targetAccount;
        return true;
    }
    else {
        return false;
    }
}

Account* AccountStorage::getAccount() const {
    return account;
}

void AccountStorage::logOut() { account = nullptr; }
