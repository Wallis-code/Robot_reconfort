#include "Appartement.cpp"
#include <iostream>

int main(){
    Appartement appart("cartes/appartement_01.json");
    std::string gril = appart.getData()["dimensions"].dump();
    int index = gril.find("hautekr");

    std::cout << gril.substr(index, 7).compare("hauteur") << std::endl; 
    return 0;
}