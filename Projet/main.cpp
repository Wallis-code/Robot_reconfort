#include "Appartement.cpp"
#include <iostream>

int main(){
    Appartement appart("cartes/appartement_01.json");
    std::cout << appart.getData()["grille"] << std::endl; 
    return 0;
}