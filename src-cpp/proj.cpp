#include "fileHandler.hpp"
#include"Account.hpp"

int main(int argc, char* argv[]){
    std::string DB_path = "dataset/DB_accounts.txt"; 
    fileHandler file(DB_path);

    /*
    AccountManager AccountList;
    while(file.readLine()){
        file.readToken();
        std::string type = file.getToken();
        file.readToken();
        std::string id = file.getToken();
        file.readToken();
        std::string pw = file.getToken();

        //std::cout << type << " " << id << " " << pw << std::endl;

        if(!type.compare("ADMIN")){
            AccountList.addAccount(new Account(ADMIN, id, pw));
        }else{
            AccountList.addAccount(new Account(USER, id, pw));
        }
    }
    file.moveFront();
    while (file.readLine()){
        std::cout << "work!" << std::endl;
        file.readToken();
        std::string type = file.getToken();
        file.readToken();
        std::string id = file.getToken();
        file.readToken();
        std::string pw = file.getToken();

        std::cout << type << " " << id << " " << pw << std::endl;
    }
    file.clear();
    int i=0;
    file.writeToken("ADMIN\tadmin\tadmin\nUSER\tuser\tuser");
    */

    std::string s = "asfa";
    s = s.substr(0,s.size());
    std::cout << s << std::endl;
    
    
    return 0;
}


//정렬 기준: 가격, 별점, default 순서
//가격이 없는 거는 지우자.