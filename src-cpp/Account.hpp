#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <vector>
#include <string>

enum AccountType { ADMIN, USER };

class Account {
public:
    Account(AccountType accountType, std::string id, std::string pw) :accountType(accountType), id(id), pw(pw) {}
    void setId(std::string id) { this->id = id; }
    std::string getId() const { return id; }
    void setPw(std::string pw) { this->pw = pw; }
    std::string getPw() const { return pw; }
    AccountType getAccountType() const { return accountType; }
private:
	AccountType accountType;
	std::string id, pw;
};

class AccountStorage {
private: 
    std::vector<Account*> accountList;
    Account* account;
public:
    AccountStorage() : account(nullptr) {}
    void addAccount(Account* account) { accountList.push_back(account); }
    //remove 함수 수정해야함. iterator 정확한 사용법 숙지할 필요.
    void removeAccount(Account* account){ 
        for(std::vector<Account*>::iterator it = accountList.begin();it != accountList.end();it++){
            size();
            accountList.erase(accountList.begin());
            //std::cout << *it.getId() << std::endl;
            //if(account -> getId().compare(it -> getId()) && account -> getPw().compare(*it -> getPw()))
        }
    }
    int size() { std::cout << "Account : " << accountList.size() << std::endl; return accountList.size(); }
    Account* find(std::string id, std::string pw) {
        for (Account* account : accountList) {
            if (!(account->getId().compare(id)) && !(account->getPw().compare(pw))) {
                return account;
            }
        }
        return nullptr;
    }
    bool login(std::string id, std::string pw) {
        Account* targetAccount = find(id, pw);
        if (targetAccount != nullptr) {
            account = targetAccount;
            return true;
        }
        else {
            return false;
        }
    }
    Account* getAccount() const {
        return account;
    }
    void logOut() { account = nullptr; }
};

#endif