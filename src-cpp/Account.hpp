#ifndef ACCOUNT_H
#define ACCOUNT_H

#include<iostream>
#include<vector>
#include<string>

enum AccountType{ADMIN, USER};

class Account {
public:
	Account(AccountType accountType, std::string id, std::string pw) :accountType(accountType), id(id), pw(pw) {};
	void setId(std::string id) { this->id = id; }
	std::string getId() const{ return id; }
	std::string getPw() const{ return pw; }
	AccountType getAccountType() const{ return accountType; }
private:
	AccountType accountType;
	std::string id, pw;
};

class AccountManager{
private: 
    std::vector<Account*> AccountList;
    Account* curAccount;
public:
    AccountManager(): curAccount(nullptr){}
    void addAccount(Account* item){ AccountList.push_back(item); }
    //remove 함수 수정해야함. iterator 정확한 사용법 숙지할 필요.
    void removeAccount(Account* item){ 
        for(std::vector<Account*>::iterator it = AccountList.begin();it != AccountList.end();it++){
            size();
            AccountList.erase(AccountList.begin());
            //std::cout << *it.getId() << std::endl;
            //if(item -> getId().compare(it -> getId()) && item -> getPw().compare(*it -> getPw()))
        }
    }
    void size(){ std::cout << "Account : " << AccountList.size() << std::endl;}
    Account* find(std::string id, std::string pw){
        for(Account* item: AccountList){
            if(!(item->getId().compare(id)) && !(item -> getPw().compare(pw))){
                return item;
            }
        }
        return nullptr;
    }
    bool login(std::string id, std::string pw){
        Account* targetAccount = find(id, pw); 
        if( targetAccount!= nullptr){
            curAccount = targetAccount;
            return true;
        }else{
            return false;
        }
    }
    Account* getAccount() const{
        return curAccount;
    }
    void logOut(){ curAccount = nullptr; }
};

#endif