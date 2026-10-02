#include "Armoire.cpp"
#include <iostream>

int main()
{
    Armoire armoire("donnees/armoire_standard.json");
    json data = armoire.getData();    
    std::cout << armoire.getCasier_depart() << std::endl;

    return 0;
}