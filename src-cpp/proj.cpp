#include "FileHandler.hpp"
#include"Account.hpp"
#include<cstdlib>
#include<ctime>

int main(int argc, char* argv[]){
    srand(time(0));
    int virtualDiscountRate = (rand()%5)*5;

    std::cout << "random2 : " <<  virtualDiscountRate << std::endl;

    return 0;
}
