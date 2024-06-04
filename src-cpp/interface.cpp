#include<iostream>
#include<vector>
#include<string>
#include<windows.h>
#include<memory>
#include<fstream>
#include<sstream>

using namespace std;

// 수정함. -> i browser.hpp
class Account {
public:
	Account(int accountType, string id, string pw) :accountType(accountType), id(id), pw(pw) {};
	void setId(string id) {
		this->id = id;
	}
	string getId() {
		return id;
	}
	string getPw() {
		return pw;
	}
	int getAccountType() {
		return accountType;
	}
private:
	int accountType;
	string id, pw;
};

/*class AdminAccount :public Account {
public:
	AdminAccount(string id, string pw) :Account(1, id, pw) {};
};

class UserAccount :public Account {
public:
	UserAccount(string id, string pw) :Account(2, id, pw) {};
};*/

/*class SignUp {
public:
	SignUp(vector<Account*>accounts) :accounts(accounts) {};
	bool signUp(int accountType, string id, string pw) {
		for (Account* account : accounts) {
			if (!id.compare(account->getId()))
				return 1;
		}
		if (accountType == 1)
			newAccount = new AdminAccount(id, pw);
		else
			newAccount = new UserAccount(id, pw);
		accounts.push_back(newAccount);
		return 0;
	}
private:
	vector<Account*> accounts;
	Account* newAccount;
};*/

/*class SignIn {
public:
	SignIn(vector<Account*>accounts) :accounts(accounts) {};
	Account* signIn(string id, string pw) {
		for (Account* account : accounts)
			if (!id.compare(account->getId()) && !pw.compare(account->getPw()))
				return account;
		throw(0);
	}
private:
	vector<Account*> accounts;
};*/


// 아래 두개는 다시 구현해야 됨.
void readAccountFile(vector<Account*> &accounts) {
	ifstream accountFile;
	accountFile.open("accounts.txt", ios::in);
	if (accountFile.fail())
		cout << "Cannot open account file" << endl;
	if (accountFile.is_open()) {
		string line, token, id, pw;
		int accountType;
		while (getline(accountFile, line)) {
			stringstream ss(line);
			for (int i = 0; getline(ss, token, '\t'); i++) {
				if (i == 0)
					accountType = stoi(token);
				else if (i == 1)
					id = token;
				else if (i == 2)
					pw = token;
			}
			Account account(accountType, id, pw);
			accounts.push_back(&account);
		}
	}
	accountFile.close();
	return;
}

void addAccountFile(int accountType, string id, string pw) {
	ofstream accountFile;
	vector<Account*> accounts;
	accountFile.open("accounts.txt", ios::app);
	if (accountFile.fail())
		cout << "Cannot open account file" << endl;
	else if (accountFile.is_open())
		accountFile << accountType << '\t' << id << '\t' << pw << endl;
	accountFile.close();
	return;
}

// in browser 
class Page {
public:
	virtual void print() = 0;
	virtual string getName() const = 0;
};

// 구현함. in browser
class MainPage :public Page {
public:
	MainPage() {
		shared_ptr<SignUpPage*> signUpPage = make_shared<SignUpPage*>();
		options.push_back(*signUpPage);
		shared_ptr<SignInPage*> signInPage = make_shared<SignInPage*>();
		options.push_back(*signInPage);
	}
	void print() override {
		int input;
		do {
			system("cls");
			cout << getName() << endl << endl;
			for (size_t i = 0; i < options.size(); ++i)
				cout << i << ". " << options[i]->getName() << std::endl;
			cout << "0. exit" << endl << endl;
			cout << "Choose an option: ";
			cin >> input;
			if (input > 0 && input <= static_cast<int>(options.size()))
				options[input - 1]->print();
		} while (input);
	}
	string getName() const override {
		return name;
	}
private:
	vector<Page*> options;
	string name = "Delivery Recommending";
};


class SignUpPage :public Page {
public:
	SignUpPage() {
		readAccountFile(accounts);
		//shared_ptr<SignUp*> signUp = make_shared<SignUp*>(accounts);
	}
	void print() override {
		int accountType;
		string id, pw;
		do {
			system("cls");
			cout << getName() << endl << endl;
			cout << "Type of Account(0 to exit, 1 for Admin, 2 for User): ";
			cin >> accountType;
			cout << "Set your ID: ";
			cin >> id;
			cout << "Set your PW: ";
			cin >> pw;
			if (accountType) {
				if (signUp(accountType, id, pw)) {
					cout << endl << "Same ID already exists." << endl;
					system("pause");
				}
				else {
					cout << endl << "Account Created." << endl;
					accountType = 0;
					system("pause");
				}
			}
		} while (accountType);
	}
	bool signUp(int accountType, string id, string pw) {
		for (Account* account : accounts)
			if (!id.compare(account->getId()))
				return 1;
		addAccountFile(accountType, id, pw);
		return 0;
	}
	string getName() const override {
		return name;
	}
private:
	vector<Account*> accounts;
	//SignUp* signUp;
	string name = "Sign Up Page";
};

class SignInPage :public Page {
public:
	SignInPage() {
		readAccountFile(accounts);
		//shared_ptr<SignIn*> signIn = make_shared<SignIn*>(accounts);
	}
	void print() override {
		string id, pw;
		do {
			system("cls");
			cout << getName() << endl << endl;
			cout << "ID: ";
			cin >> id;
			cout << "PW: ";
			cin >> pw;
			try {
				account = signIn(id, pw);
				shared_ptr<AdminMainPage*> adminMain = make_shared<AdminMainPage*>(account);
				options.push_back(*adminMain);
				shared_ptr<UserMainPage*> userMain = make_shared<UserMainPage*>(account);
				options.push_back(*userMain);
				options[account->getAccountType() - 1]->print();
			}
			catch (int i) {
				cout << "No matching account exists." << endl;
				system("pause");
			}
		} while (1);
	}
	Account* signIn(string id, string pw) {
		for (Account* account : accounts)
			if (!id.compare(account->getId()) && !pw.compare(account->getPw()))
				return account;
		throw(0);
	}
	string getName() const override {
		return name;
	}
private:
	//SignIn* signIn;
	Account* account;
	vector<Account*> accounts;
	vector<Page*> options;
	string name = "Sign In Page";
};

class AdminMainPage :public Page {
public:
	AdminMainPage(Account* account) :account(account) {
		shared_ptr<ModifyRestaurant*> modifyRestaurant = make_shared<ModifyRestaurant*>();
		options.push_back(*modifyRestaurant);
		shared_ptr<AccountInfoPage*> accouontInfoPage = make_shared<AccountInfoPage*>(account);
		options.push_back(*accouontInfoPage);
	}
	void print() override {
		int input;
		do {
			system("cls");
			cout << getName() << endl << endl;
			for (size_t i = 0; i < options.size(); ++i)
				cout << i << ". " << options[i]->getName() << std::endl;
			cout << "0. exit" << endl << endl;
			cout << "Choose an option: ";
			cin >> input;
			if (input > 0 && input <= static_cast<int>(options.size()))
				options[input - 1]->print();
		} while (input);
	}
	string getName() const override {
		return name;
	}
private:
	Account* account;
	vector<Page*> options;
	string name = "Admin Main Page";
};

//구현해야됨.
class ModifyRestaurant :public Page {
public:
	ModifyRestaurant() {};
	void print() override {
		string input;
		Store* targetRestaurant;
		do {
			system("cls");
			cout << getName() << endl << endl;
			cout << "Input restaurant's name to modify(0 to exit): ";
			cin >> input;
			for (size_t i = 0; i < restaurants.size(); ++i)
				if (!input.compare(restaurants[i]->getName())) {
					targetRestaurant = restaurants[i];
					break;
				}
			system("cls");
			for (int i = 0; i < targetRestaurant->menuSize(); ++i)
				cout << i << ". " << targetRestaurant->menuList()->getName() << std::endl;
			cout << "Number for a menu, 0 to exit : ";
			cin >> input;
			if (stoi(input) > 0 && stoi(input) <= static_cast<int>(targetRestaurant.menuSize()))
				targetRestaurant->menuList[stoi(input) - 1]->print();
		} while (stoi(input));
	}
	string getName() const override {
		return name;
	}
private:
	vector<Store*> restaurants;
	string name = "Restaurant Modifying Page";
};
//구현함.

class UserMainPage :public Page {
public:
	UserMainPage(Account* account) :account(account) {
		shared_ptr<SearchPage*> searchPage = make_shared<SearchPage*>();
		options.push_back(*searchPage);
		shared_ptr<CategoriesPage*> cateroriesPage = make_shared<CategoriesPage*>();
		options.push_back(*cateroriesPage);
		shared_ptr<OrderHistoryPage*> orderHistoryPage = make_shared<OrderHistoryPage*>(account);
		options.push_back(*orderHistoryPage);
		shared_ptr<FavoritesPage*> favoritesPage = make_shared<FavoritesPage*>(account);
		options.push_back(*favoritesPage);
		shared_ptr<AccountInfoPage*> accountInfoPage = make_shared<AccountInfoPage*>(account);
		options.push_back(*accountInfoPage);
	}
	void print() override {
		int input;
		do {
			system("cls");
			cout << getName() << endl << endl;
			for (size_t i = 0; i < options.size(); ++i)
				cout << i << ". " << options[i]->getName() << std::endl;
			cout << "0. exit" << endl << endl;
			cout << "Choose an option: ";
			cin >> input;
			if (input > 0 && input <= static_cast<int>(options.size()))
				options[input - 1]->print();
		} while (input);
	}
	string getName() const override {
		return name;
	}
private:
	Account* account;
	vector<Page*> options;
	string name = "User Main Page";
};

//구현 했으나 연결 안함.
class SearchPage :public Page {
public:
	SearchPage() {};
	void print() override {
		string input;
		do {
			system("cls");
			cout << getName() << endl << endl;
			cout << "Search(0 to exit): ";
			cin >> input;
			if (stoi(input))
				search(input);
		} while (stoi(input));
	}
	string getName() const override {
		return name;
	}
private:
	string name = "Search Page";
};

/*class SearchResultPage :public Page {
public:
	SearchResultPage(string input) {

	}
};*/

// 구현했으나 연결 필요.
class CategoriesPage :public Page {
public:
	CategoriesPage() {
		shared_ptr<CategoriesRestaurantsPage*> Korean = make_shared<CategoriesRestaurantsPage*>("Korean");
		options.push_back(*Korean);
		shared_ptr<CategoriesRestaurantsPage*> Chinese = make_shared<CategoriesRestaurantsPage*>("Chinese");
		options.push_back(*Chinese);
		shared_ptr<CategoriesRestaurantsPage*> Japanese = make_shared<CategoriesRestaurantsPage*>("Japanese");
		options.push_back(*Japanese);
		shared_ptr<CategoriesRestaurantsPage*> Western = make_shared<CategoriesRestaurantsPage*>("Western");
		options.push_back(*Western);
		shared_ptr<CategoriesRestaurantsPage*> Italian = make_shared<CategoriesRestaurantsPage*>("Italian");
		options.push_back(*Italian);
		shared_ptr<CategoriesRestaurantsPage*> FastFood = make_shared<CategoriesRestaurantsPage*>("Fast Food");
		options.push_back(*FastFood);
		shared_ptr<CategoriesRestaurantsPage*> Beverages = make_shared<CategoriesRestaurantsPage*>("Beverages");
		options.push_back(*Beverages);
		shared_ptr<CategoriesRestaurantsPage*> Desserts = make_shared<CategoriesRestaurantsPage*>("Desserts");
		options.push_back(*Desserts);
	}
	void print() override {
		int input;
		do {
			system("cls");
			cout << getName() << endl << endl;
			for (size_t i = 0; i < options.size(); ++i)
				cout << i << ". " << options[i]->getName() << std::endl;
			cout << "0. exit" << endl << endl;
			cout << "Choose an option: ";
			cin >> input;
			if (input > 0 && input <= static_cast<int>(options.size()))
				options[input - 1]->print();
		} while (input);
	}
	string getName() const override {
		return name;
	}
private:
	vector<Page*> options;
	string name = "Categories";
};

// 
class CategoriesRestaurantsPage :public Page {
public:
	CategoriesRestaurantsPage(string category) :name(category) {};
	void print() override {
		int show = 0, input;
		do {
			system("cls");
			cout << getName() << endl << endl;
			for (size_t i = 0; i < restaurants.size() && i < show * 10; ++i)
				cout << i << ". " << restaurants[i]->getName() << std::endl;
			cout << "1 ~ 10 for a restaurant, -1 to prev, -2 to next, -3 to exit: ";
			cin >> input;
			if (input > 0 && input <= static_cast<int>(restaurants.size()))
				restaurants[show * 10 + input - 1]->print();
			else if (show > 0 && input == -1)
				show--;
			else if ((show + 1) * 10 < restaurants.size() && input == -2)
				show++;
		} while (input > -3);
	}
	string getName() const override {
		return name;
	}
private:
	vector<Store*> restaurants;
	string name;
};

class OrderHistoryPage :public Page {
public:
	OrderHistoryPage(Account* account) :account(account) {};
	void print() override {
		system("cls");
		cout << getName() << endl << endl;
		ifstream orderHistoryFile;
		orderHistoryFile.open(account->getId() + "'s Order History.txt");
		if (orderHistoryFile.fail())
			cout << "Cannot open order history file." << endl;
		if (orderHistoryFile.is_open()) {
			string line, token;
			while (getline(orderHistoryFile, line)) {
				stringstream ss(line);
				for (int i = 0; getline(ss, token, '\t'); i++)
					cout << token << "\t";
				cout << endl;
			}
			system("pause");
		}
	}
	string getName() const override {
		return name;
	}
private:
	Account* account;
	string name = "My Order History";
};

class FavoritesPage :public Page {
public:
	FavoritesPage(Account* account) :account(account) {};
	void print() override {
		system("cls");
		cout << getName() << endl << endl;
		ifstream favoritesFile;
		favoritesFile.open(account->getId() + "'s Favorites.txt");
		if (favoritesFile.fail())
			cout << "Cannot open order history file." << endl;
		if (favoritesFile.is_open()) {
			string line, token;
			while (getline(favoritesFile, line)) {
				stringstream ss(line);
				for (int i = 0; getline(ss, token, '\t'); i++)
					cout << token << "\t";
				cout << endl;
			}
			system("pause");
		}
	}
	string getName() const override {
		return name;
	}
private:
	Account* account;
	string name = "My Favorites";
};

class AccountInfoPage :public Page {
public:
	AccountInfoPage(Account* account) :account(account) {};
	void print() override {
		system("cls");
		cout << getName() << endl << endl;
		if (account->getAccountType() == 1)
			cout << "Account Type: Admin" << endl;
		else
			cout << "Account Type: User" << endl;
		cout << "ID: " << account->getId() << endl;
		cout << "PW: " << account->getPw() << endl << endl;
		system("pause");
	}
	string getName() const override {
		return name;
	}
private:
	Account* account;
	string name = "Account Info";
};

int main() {
	MainPage mainPage;
	mainPage.print();
}