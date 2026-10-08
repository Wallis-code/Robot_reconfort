#include "Armoire.hpp"
#include <iostream>

int main()
{
    Armoire armoire("donnees/armoire_standard.json");
    json data = armoire.getData();    
    
    (armoire.getCasierCase(0 , 7)).print();

    return 0;
}