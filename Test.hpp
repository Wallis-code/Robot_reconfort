#include "Grille.hpp"
#include "Parcours.hpp"
#include "Dictionnaire.hpp"
#include "Armoire.hpp"
#include "Demande.hpp"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

void testPlanification(const std::string& cheminGrille);
void testReplanification(const std::string& cheminGrilleInconnue, const std::string& cheminGrilleMur);
void testDictionnaire(const std::string& cheminMessages, const std::string& cheminDictionnaire);
void testResidentEnmure(const std::string& cheminGrille, std::string idResident);