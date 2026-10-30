#include "Test.hpp"

int main() {
    testPlanification("test/grille_test1.json");
    testReplanification("test/grille_replanification_inconnue.json",
                        "test/grille_replanification_mur.json");
    return 0;
}
