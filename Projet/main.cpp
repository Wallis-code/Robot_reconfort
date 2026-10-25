#include "Appartement.cpp"
#include <iostream>

int main(){
    Appartement appart("cartes/appartement_01.json");
    Grille grille = appart.getGrille();
    std::cout << "Dimensions de l'appartement : " << grille.getHauteur() << " x " << grille.getLargeur() << std::endl;
    return 0;
}