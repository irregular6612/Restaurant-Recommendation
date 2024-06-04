#include<vector>
#include<iostream>
#include<functional>

class Command {
public:
    virtual void execute() = 0;
    virtual std::string getName() const = 0;
};

template<typename T>
class MenuItemCommand : public Command{
private:
    std::string name;
    T action;
public:
    MenuItemCommand(const std::string& name, T& action): name(name), action(action){}
    void execute() override{ action.action(); }
    std::string getName() const override{ return name; }
};

class Menu{
private:
    std::vector<Command*> items;
public:
    void addItem(Command* item){ items.push_back(item); }
    void show(){
        int choice = 0;

        do{
            std::cout << "Menu:\n";
            for(int i=0;i<items.size();i++){
                std::cout << i + 1 << items[i] -> getName() << std::endl;
            }
            std::cin >> choice;
        } while (choice != 0);
        
    }
};


class Receiver1{
private:
public:
    Receiver1(){}
    void action(){
        std::cout << "execute 1"<< std::endl;
    }
};


int main(){
    Menu menu;
    Receiver1 r;
    MenuItemCommand Command1("Action1", r);
    menu.addItem(&Command1);
    menu.show();
    return 0;
}