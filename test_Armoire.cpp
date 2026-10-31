#include <cassert>
#include <iostream>
#include <vector>
#include "Armoire.hpp"
#include "Candidat.hpp"
 
static const char* FICHIER_ARMOIRE = "donnees/armoire_standard.json";
 
static void test_distanceRoue() {
    assert(distanceRoue(0, 0) == 0);
    assert(distanceRoue(0, 1) == 1);
    assert(distanceRoue(0, 7) == 1);   // joie et anticipation sont voisines (bouclage)
    assert(distanceRoue(0, 2) == 2);
    assert(distanceRoue(0, 6) == 2);
    assert(distanceRoue(0, 4) == 4);   // joie et tristesse sont opposées
    assert(distanceRoue(4, 0) == 4);   // symétrique
    std::cout << "OK distanceRoue" << std::endl;
}
 
// Exemple de l'annexe C.5 : forte (2) / surprise (3)
static void test_candidats_exemple_C5() {
    std::vector<Candidat> l = candidats(2, 3);
    int attendu[15][2] = {
        {2,3}, {1,3}, {0,3},
        {2,4}, {2,2}, {1,4}, {1,2}, {0,4}, {0,2},
        {2,5}, {2,1}, {1,5}, {1,1}, {0,5}, {0,1}
    };
    assert(l.size() == 15);
    for (int i = 0; i < 15; i++) {
        assert(l[i].ligne == attendu[i][0]);
        assert(l[i].colonne == attendu[i][1]);
    }
    std::cout << "OK candidats (exemple C.5)" << std::endl;
}
 
// moyenne (1) / joie (0) : les voisines passent par le bouclage 0 <-> 7
static void test_candidats_bouclage() {
    std::vector<Candidat> l = candidats(1, 0);
    int attendu[7][2] = {
        {1,0},           // casier exact
        {0,0}, {2,0},    // même émotion, faible avant forte
        {1,1}, {1,7},    // voisines : horaire (confiance) avant antihoraire (anticipation)
        {0,1}, {0,7}
    };
    for (int i = 0; i < 7; i++) {
        assert(l[i].ligne == attendu[i][0]);
        assert(l[i].colonne == attendu[i][1]);
    }
    std::cout << "OK candidats (bouclage des colonnes)" << std::endl;
}
 
// Aucun candidat à distance 3 ou 4, pour toutes les demandes possibles
static void test_candidats_jamais_trop_loin() {
    for (int intensite = 0; intensite < 3; intensite++) {
        for (int emotion = 0; emotion < 8; emotion++) {
            std::vector<Candidat> l = candidats(intensite, emotion);
            assert(l.size() == 15);
            for (const Candidat& c : l)
                assert(distanceRoue(emotion, c.colonne) <= 2);
        }
    }
    std::cout << "OK candidats (jamais à distance > 2)" << std::endl;
}
 
// Sélecteur : départ en [1,0] dans armoire_standard.json
static void test_chercher_bouclage() {
    Armoire a(FICHIER_ARMOIRE);
    assert(a.contenuCasierCourant() == "photo de famille");   // [1,0]
 
    assert(a.chercher(O));                                   // colonne 0 -> 7
    assert(a.contenuCasierCourant() == "calendrier");        // [1,7]
 
    assert(a.chercher(E));                                   // colonne 7 -> 0
    assert(a.contenuCasierCourant() == "photo de famille");  // [1,0]
    std::cout << "OK chercher (bouclage 7 <-> 0)" << std::endl;
}
 
static void test_chercher_bords_lignes() {
    Armoire a(FICHIER_ARMOIRE);
 
    assert(a.chercher(N));                                   // ligne 1 -> 0
    assert(a.contenuCasierCourant() == "tisane");            // [0,0]
    assert(!a.chercher(N));                                  // interdit depuis la ligne 0
    assert(a.contenuCasierCourant() == "tisane");            // le sélecteur n'a pas bougé
 
    assert(a.chercher(S));
    assert(a.chercher(S));                                   // ligne 0 -> 2
    assert(a.contenuCasierCourant() == "guirlande lumineuse"); // [2,0]
    assert(!a.chercher(S));                                  // interdit depuis la ligne 2
    assert(a.contenuCasierCourant() == "guirlande lumineuse");
    std::cout << "OK chercher (lignes qui ne bouclent pas)" << std::endl;
}
 
static void test_casier_vide() {
    Armoire a(FICHIER_ARMOIRE);
    assert(a.chercher(E));
    assert(a.chercher(E));
    assert(a.chercher(E));                                   // [1,3] surprise moyenne
    assert(a.contenuCasierCourant().empty());
    std::cout << "OK casier vide" << std::endl;
}
 
int main() {
    test_distanceRoue();
    test_candidats_exemple_C5();
    test_candidats_bouclage();
    test_candidats_jamais_trop_loin();
    test_chercher_bouclage();
    test_chercher_bords_lignes();
    test_casier_vide();
    std::cout << "Tous les tests armoire passent." << std::endl;
    return 0;
}
