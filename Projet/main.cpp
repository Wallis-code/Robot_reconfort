#include "Appartement.cpp"
#include <iostream>
#include <cassert>

void testerClasseCase() {
    std::cout << "--- Debut des tests de la classe Case ---" << std::endl;

    // 1. Test d'instanciation directe des classes filles
    CaseMur mur;
    assert(mur.getSymbole() == '#');
    assert(mur.estTraversable() == false);

    CaseLibre libre;
    assert(libre.getSymbole() == '.');
    assert(libre.estTraversable() == true);

    CaseDepart depart;
    assert(depart.getSymbole() == 'R');
    assert(depart.estTraversable() == true);

    CaseArmoire armoire;
    assert(armoire.getSymbole() == 'A');
    assert(armoire.estTraversable() == false);

    CaseDictionnaire dico;
    assert(dico.getSymbole() == 'D');
    assert(dico.estTraversable() == false);

    CaseResident resident;
    assert(resident.getSymbole() == 'P');
    assert(resident.estTraversable() == false);

    std::cout << "[OK] Instanciation directe et attributs (symbole / traversable)" << std::endl;

    // 2. Test de la fabrique (creerCase) et du polymorphisme
    auto ptrLibre = creerCase('.');
    assert(ptrLibre != nullptr);
    assert(ptrLibre->getSymbole() == '.');
    assert(ptrLibre->estTraversable() == true);

    auto ptrArmoire = creerCase('A');
    assert(ptrArmoire != nullptr);
    assert(ptrArmoire->getSymbole() == 'A');
    assert(ptrArmoire->estTraversable() == false);

    // Appel polymorphe de interagir()
    ptrLibre->interagir();
    ptrArmoire->interagir();

    std::cout << "[OK] Fabrique creerCase() et polymorphisme" << std::endl;

    // 3. Test de détection d'un symbole invalide
    bool exceptionLevee = false;
    try {
        auto ptrInvalide = creerCase('X'); // 'X' n'existe pas dans la légende
    } catch (const std::invalid_argument& e) {
        exceptionLevee = true;
    }
    assert(exceptionLevee == true);

    std::cout << "[OK] Gestion d'erreur sur symbole inconnu" << std::endl;
    std::cout << "--- Tous les tests de Case ont reussi ! ---\n" << std::endl;
}

void testerClasseGrille(const json& dataAppart) {
    std::cout << "--- Debut des tests de la classe Grille ---" << std::endl;

    // 1. Construction de la grille à partir du JSON de appartement_01.json
    Grille g(dataAppart);

    // 2. Vérification des cases spécifiques de appartement_01.json
    // Ligne 0 : "#############" -> Coin supérieur gauche (0, 0) et droit (0, 12)
    assert(g.getCase(Position(0, 0)).getSymbole() == '#');
    assert(g.getCase(Position(0, 0)).estTraversable() == false);
    assert(g.getCase(Position(0, 12)).getSymbole() == '#');

    // Ligne 1 : "#......#....#" -> Case libre en (1, 1) et mur interne en (1, 7)
    assert(g.getCase(Position(1, 1)).getSymbole() == '.');
    assert(g.getCase(Position(1, 1)).estTraversable() == true);
    assert(g.getCase(Position(1, 7)).getSymbole() == '#');
    assert(g.getCase(Position(1, 7)).estTraversable() == false);

    // Ligne 3 : "#..P...#....#" -> Premier résident 'P' en (3, 3)
    assert(g.getCase(Position(3, 3)).getSymbole() == 'P');
    assert(g.getCase(Position(3, 3)).estTraversable() == false);

    // Ligne 6 : "#R..A..#...P#" -> Robot 'R' en (6, 1), Armoire 'A' en (6, 4), Résident 'P' en (6, 11)
    assert(g.getCase(Position(6, 1)).getSymbole() == 'R');
    assert(g.getCase(Position(6, 1)).estTraversable() == true);

    assert(g.getCase(Position(6, 4)).getSymbole() == 'A');
    assert(g.getCase(Position(6, 4)).estTraversable() == false);

    assert(g.getCase(Position(6, 11)).getSymbole() == 'P');
    assert(g.getCase(Position(6, 11)).estTraversable() == false);

    // Ligne 7 : "#.....D#....#" -> Dictionnaire 'D' en (7, 6)
    assert(g.getCase(Position(7, 6)).getSymbole() == 'D');
    assert(g.getCase(Position(7, 6)).estTraversable() == false);

    std::cout << "[OK] Lecture et placement exacts de toutes les cases de appartement_01.json" << std::endl;

    // 3. Test de modification d'une case avec setY()
    // On remplace la case libre (1, 1) par une armoire 'A'
    g.setY(Position(1, 1), 'A');
    assert(g.getCase(Position(1, 1)).getSymbole() == 'A');
    assert(g.getCase(Position(1, 1)).estTraversable() == false);

    // On la remet en case libre '.'
    g.setY(Position(1, 1), '.');
    assert(g.getCase(Position(1, 1)).getSymbole() == '.');
    assert(g.getCase(Position(1, 1)).estTraversable() == true);

    std::cout << "[OK] Modification dynamique d'une case avec setY()" << std::endl;

    // 4. Test avec des dimensions inversées dans le JSON ("largeur" avant "hauteur")
    json jsonInverse = R"({
        "dimensions": { "largeur": 3, "hauteur": 2 },
        "grille": [
            "###",
            "#R."
        ]
    })"_json;

    Grille gInverse(jsonInverse);
    assert(gInverse.getCase(Position(1, 1)).getSymbole() == 'R');
    assert(gInverse.getCase(Position(1, 2)).getSymbole() == '.');

    std::cout << "[OK] Support des dimensions quel que soit l'ordre des cles JSON" << std::endl;

    // 5. Test de détection d'une grille aux dimensions erronées (doit afficher une erreur sans crasher)
    std::cout << "Test d'erreur attendue (dimensions non concordantes) :" << std::endl;
    json jsonInvalide = R"({
        "dimensions": { "hauteur": 5, "largeur": 13 },
        "grille": [
            "#############",
            "#...........#"
        ]
    })"_json;
    Grille gInvalide(jsonInvalide); // Doit afficher l'erreur de hauteur sur std::cerr

    std::cout << "--- Tous les tests de Grille ont reussi ! ---\n" << std::endl;
}

int main() {
    // Exécution des tests unitaires sur Case
    testerClasseCase();

    // Test du chargement de l'appartement    
    Appartement appart("cartes/appartement_01.json");
    if (!appart.getData().is_null()) {
        testerClasseGrille(appart.getData());
    }
    Grille grille = appart.getGrille();
    std::cout << "Dimensions de l'appartement : " << grille.getHauteur() << " x " << grille.getLargeur() << std::endl;
    return 0;
}