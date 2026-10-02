#include "Appartement.cpp"
#include <iostream>

int main(){
    Appartement appart;
    json data = appart.open("cartes/appartement_01.json");
    std::cout << data.dump(4) << std::endl; 
    return 0;
}