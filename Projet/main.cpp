#include "Appartement.cpp"
#include "Armoire.hpp"
#include "Candidat.hpp"
#include "Demande.hpp"
#include <iostream>

int main()
{
    Demande demande(1, "ID123", "Élève très âgé");
    std::cout << "Demande : " << demande.getNumero() << ", " << demande.getIdRes() << ", " << demande.getMessage() << std::endl;

    Armoire armoire("donnees/armoire_standard.json");
    json data = armoire.getData();    
    
    for (const Candidat& c : candidats(3, 2))
        std::cout << "[" << c.ligne << "," << c.colonne << "] " << std::endl;

    // Test appartement    
    Appartement appart("cartes/appartement_01.json");
    Grille grille = appart.getGrille();
    std::cout << "Dimensions de l'appartement : " << grille.getHauteur() << " x " << grille.getLargeur() << std::endl;

    return 0;
}