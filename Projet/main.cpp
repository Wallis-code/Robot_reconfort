#include "Appartement.cpp"
#include "Armoire.hpp"
#include "Candidat.hpp"
#include "Demande.hpp"
#include "Dictionnaire.hpp"
#include "Scenario.hpp"
#include "Robot.hpp"
#include <iostream>

int main()
{
    Dictionnaire dictionnaire("donnees/dictionnaire.json");

    Demande demande(1, "ID123", "Élève très sidérée");
    std::cout << "Demande : " << demande.getNumero() << ", " << demande.getIdRes() << ", " << demande.getMessage() << std::endl;

    std::pair<std::string, std::string> resultat = dictionnaire.consulter(demande.getMessage());
    std::cout << "Résultat : " << resultat.first << ", " << resultat.second << std::endl;
    
    Scenario scenario("cartes/scenario_01.json");

    const std::vector<Demande>& demandes = scenario.getDemandes();
    for (const Demande& d : demandes) {
        std::cout << "Demande : " << d.getNumero() << ", " << d.getIdRes() << ", " << d.getMessage() << std::endl;
    }

    Armoire armoire("donnees/armoire_standard.json");
    json data = armoire.getData();    
    
    //for (const Candidat& c : candidats(3, 2))
    //    std::cout << "[" << c.ligne << "," << c.colonne << "] " << std::endl;

    // Test appartement    
    Appartement appart("cartes/appartement_01.json");
    Grille grille = appart.getGrille();
    grille.afficher();
    std::cout << "Dimensions de l'appartement : " << grille.getHauteur() << " x " << grille.getLargeur() << std::endl;

    Robot robot(grille);
    robot.getGrille().afficher();

    return 0;
}