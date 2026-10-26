#include "Armoire.hpp"
#include <iostream>
#include "Candidat.hpp"

int main()
{
    Armoire armoire("donnees/armoire_standard.json");
    json data = armoire.getData();    
    

    return 0;
}