#include "Test.hpp"

int main() {
    testPlanification("test/grille_test1.json");
    testReplanification("test/grille_replanification_inconnue.json", "test/grille_replanification_mur.json");
    testDictionnaire("test/messages_dictionnaire.txt", "donnees/dictionnaire.json");
    testResidentEnmure("test/appartement_03_copy.json", "R1");
    return 0;
}
