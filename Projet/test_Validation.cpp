// Tests de validation (section 8, point 6) : un fichier cassé par catégorie
// On part d'un jeu valide construit en mémoire, et on casse une seule chose à chaque fois.
#include <cassert>
#include <iostream>
#include "VerificationCoherenceJSON.hpp"

static json carteValide() {
    return {
        {"format", "robot-reconfort/carte"}, {"version", 1}, {"nom", "carte_test"},
        {"dimensions", {{"hauteur", 4}, {"largeur", 5}}},
        {"grille", {"#####", "#R.D#", "#A.P#", "#####"}},
        {"depart_robot", {1, 1}},
        {"armoire", {{"position", {2, 1}}}},
        {"dictionnaire", {{"position", {1, 3}}}},
        {"residents", {{{"id", "R1"}, {"nom", "Alice"}, {"position", {2, 3}}}}}
    };
}

static json armoireValide() {
    json a = {{"format", "robot-reconfort/armoire"}, {"version", 1}, {"nom", "armoire_test"},
              {"casier_depart", {1, 0}}, {"casiers", json::array()}};
    for (int l = 0; l < 3; l++)
        for (int c = 0; c < 8; c++)
            a["casiers"].push_back({{"ligne", l}, {"colonne", c}, {"emotion", "joie"},
                                    {"intensite", "faible"}, {"objet", "tisane"}});
    return a;
}

static json dicoValide() {
    return {
        {"format", "robot-reconfort/dictionnaire"}, {"version", 1}, {"nom", "dico_test"},
        {"emotions", {"joie", "confiance", "peur", "surprise",
                      "tristesse", "degout", "colere", "anticipation"}},
        {"intensites", {"faible", "moyenne", "forte"}},
        {"entrees", {{{"formes", {"content", "contente"}}, {"emotion", "joie"}, {"intensite", "faible"}}}}
    };
}

static json scenarioValide() {
    return {
        {"format", "robot-reconfort/scenario"}, {"version", 1}, {"nom", "scenario_test"},
        {"carte", "carte_test"}, {"armoire", "armoire_test"},
        {"demandes", {{{"numero", 1}, {"resident", "R1"}, {"message", "je suis content"}}}}
    };
}

static bool valide(const json& carte, const json& scenario, const json& armoire, const json& dico) {
    return VerificationCoherenceJSON::verifier(carte, scenario, armoire, dico);
}

static void test_jeu_valide() {
    assert(valide(carteValide(), scenarioValide(), armoireValide(), dicoValide()));
    std::cout << "OK validation : jeu de référence accepté" << std::endl;
}

static void test_carte_cassee() {
    json c = carteValide();
    c["format"] = "robot-reconfort/plan";                       // mauvais format
    assert(!valide(c, scenarioValide(), armoireValide(), dicoValide()));

    c = carteValide();
    c["grille"][1] = "#RRD#";                                    // deux départs
    assert(!valide(c, scenarioValide(), armoireValide(), dicoValide()));

    c = carteValide();
    c["residents"][0]["position"] = {2, 2};                      // résident sur une case sans P
    assert(!valide(c, scenarioValide(), armoireValide(), dicoValide()));
    std::cout << "OK validation : carte cassée refusée" << std::endl;
}

static void test_armoire_cassee() {
    json a = armoireValide();
    a["casiers"][0]["ligne"] = 3;                                // casier hors de l'armoire
    assert(!valide(carteValide(), scenarioValide(), a, dicoValide()));

    a = armoireValide();
    a["casiers"].erase(a["casiers"].end() - 1);                  // 23 casiers au lieu de 24
    assert(!valide(carteValide(), scenarioValide(), a, dicoValide()));

    a = armoireValide();
    a["casiers"][1]["colonne"] = 0;                              // deux casiers au même endroit
    assert(!valide(carteValide(), scenarioValide(), a, dicoValide()));
    std::cout << "OK validation : armoire cassée refusée" << std::endl;
}

static void test_dictionnaire_casse() {
    json d = dicoValide();
    d["version"] = 2;                                            // mauvaise version
    assert(!valide(carteValide(), scenarioValide(), armoireValide(), d));

    d = dicoValide();
    d["entrees"][0]["formes"] = json::array();                   // entrée sans forme
    assert(!valide(carteValide(), scenarioValide(), armoireValide(), d));
    std::cout << "OK validation : dictionnaire cassé refusé" << std::endl;
}

static void test_scenario_casse() {
    json s = scenarioValide();
    s["demandes"][0]["resident"] = "R9";                         // résident inconnu de la carte
    assert(!valide(carteValide(), s, armoireValide(), dicoValide()));

    s = scenarioValide();
    s["demandes"].push_back({{"numero", 1}, {"resident", "R1"}, {"message", "re"}});   // numéro en double
    assert(!valide(carteValide(), s, armoireValide(), dicoValide()));

    s = scenarioValide();
    s["carte"] = "autre_carte";                                  // ne correspond pas à la carte fournie
    assert(!valide(carteValide(), s, armoireValide(), dicoValide()));
    std::cout << "OK validation : scénario cassé refusé" << std::endl;
}

int main() {
    test_jeu_valide();
    test_carte_cassee();
    test_armoire_cassee();
    test_dictionnaire_casse();
    test_scenario_casse();
    std::cout << "Tous les tests validation passent." << std::endl;
    return 0;
}