#include "Grille.hpp"
#include "Parcours.hpp"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

void testPlanification(const std::string& cheminGrille);
void testReplanification(const std::string& cheminGrilleInconnue,
                         const std::string& cheminGrilleMur);