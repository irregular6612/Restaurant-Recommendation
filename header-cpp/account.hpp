#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>


// account structure and its container list.
enum AccountType { ADMIN, USER };

class Account {
public:
    Account(AccountType accountType, std::string id, std::string pw);
    void setId(std::string id);
    std::string getId() const;
    void setPw(std::string pw);
    std::string getPw() const;
    AccountType getAccountType() const;
private:
	AccountType accountType;
	std::string id, pw;
};

class AccountStorage {
private: 
    std::vector<Account*> accountList;
    Account* account;
public:
    AccountStorage();
    void addAccount(Account* account);
    void removeAccount(Account* account);
    int size();
    Account* find(std::string id, std::string pw);
    bool login(std::string id, std::string pw);
    Account* getAccount() const;
    void logOut();
};

#endif