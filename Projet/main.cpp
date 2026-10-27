#include "Armoire.hpp"
#include <iostream>
#include "Candidat.hpp"

int main()
{
    Armoire armoire("donnees/armoire_standard.json");
    json data = armoire.getData();    
    
    for (const Candidat& c : candidats(3, 2))
        std::cout << "[" << c.ligne << "," << c.colonne << "] " << std::endl;

    return 0;
}