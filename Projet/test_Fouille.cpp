// Tests de la fouille (section 8, point 5) : les 4 cas de repli
// Chaque test part d'une armoire vide, remplit quelques casiers, puis lance fouiller.
// Toutes les demandes sont "moyenne joie" : le casier visé est [1,0].
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include "Armoire.hpp"
#include "Fouille.hpp"

static const char* FICHIER_TMP = "test/armoire_test_fouille.json";

// json d'une armoire de 24 casiers, tous vides, sélecteur au départ en [1,0]
// le casier [ligne, colonne] est à l'indice ligne * 8 + colonne dans "casiers"
static json armoireVide() {
    json data;
    data["format"] = "robot-reconfort/armoire";
    data["version"] = 1;
    data["nom"] = "armoire_test";
    data["casier_depart"] = {1, 0};
    data["casiers"] = json::array();
    for (int l = 0; l < 3; l++)
        for (int c = 0; c < 8; c++)
            data["casiers"].push_back({{"ligne", l}, {"colonne", c}, {"emotion", "joie"},
                                       {"intensite", "faible"}, {"objet", nullptr}});
    return data;
}

// Armoire ne se construit qu'à partir d'un fichier
//on doit faire ça car le test va créer un json qui n'est pas existant
static Armoire charger(const json& data) {
    std::ofstream f(FICHIER_TMP);
    assert(f.is_open());          
    f << data.dump();
    f.close();                   
    return Armoire(FICHIER_TMP);
}

// [1,0] garni : on le prend sans bouger
static void test_casier_exact() {
    json data = armoireVide();
    data["casiers"][1 * 8 + 0]["objet"] = "photo";
    Armoire a = charger(data);

    Fouille f = fouiller(a, 1, 0);
    assert(f.succes);
    assert(f.objet == "photo");
    assert(f.repli == AUCUN);
    assert(f.mouvements.empty());
    std::cout << "OK repli : casier exact garni" << std::endl;
}

// [1,0] vide, [0,0] garni : même émotion, autre intensité
static void test_repli_intensite() {
    json data = armoireVide();
    data["casiers"][0 * 8 + 0]["objet"] = "tisane";
    Armoire a = charger(data);

    Fouille f = fouiller(a, 1, 0);
    assert(f.succes);
    assert(f.objet == "tisane");
    assert(f.repli == INTENSITE);
    std::cout << "OK repli : intensite" << std::endl;
}

// Colonne joie vide, [1,1] garni : voisine à distance 1
static void test_repli_voisine() {
    json data = armoireVide();
    data["casiers"][1 * 8 + 1]["objet"] = "lettre";
    Armoire a = charger(data);

    Fouille f = fouiller(a, 1, 0);
    assert(f.succes);
    assert(f.objet == "lettre");
    assert(f.repli == V1);
    std::cout << "OK repli : colonne vide -> voisine" << std::endl;
}

// Aucun casier garni : armoire vide
static void test_armoire_vide() {
    Armoire a = charger(armoireVide());

    Fouille f = fouiller(a, 1, 0);
    assert(!f.succes);
    assert(f.objet.empty());
    std::cout << "OK repli : aucun casier garni (armoire vide)" << std::endl;
}

int main() {
    test_casier_exact();
    test_repli_intensite();
    test_repli_voisine();
    test_armoire_vide();
    std::cout << "Tous les tests fouille passent." << std::endl;
    return 0;
}