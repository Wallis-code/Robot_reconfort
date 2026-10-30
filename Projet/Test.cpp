#include "Test.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


// Fonctions utilitaires pour les tests

static Grille chargerGrille(const std::string& cheminGrille) {
    std::ifstream fichier(cheminGrille);
    assert(fichier.is_open());
    return Grille(json::parse(fichier));
}

static bool memesPositions(const std::vector<Position>& gauche,
                           const std::vector<Position>& droite) {
    if (gauche.size() != droite.size()) {
        return false;
    }
    for (std::size_t i = 0; i < gauche.size(); ++i) {
        if (!memePosition(gauche[i], droite[i])) {
            return false;
        }
    }
    return true;
}

// -------------Test planification-----------------

void testPlanification(const std::string& cheminGrille) {

    const Grille grille = chargerGrille(cheminGrille);
    const Position depart = grille.getPositionDepart();
    const Position resident = grille.getPositionResident("");
    const Position dictionnaire = grille.getPositionDictionnaire();

    const std::vector<Position> cheminResident =
        plusCourtChemin(grille, depart, resident);
    const std::vector<Position> cheminDictionnaire =
        plusCourtChemin(grille, depart, dictionnaire);

    const std::vector<Position> cheminResidentAttendu = {
        Position(3, 4), Position(2, 4), Position(1, 4)
    };
    const std::vector<Position> cheminDictionnaireAttendu = {
        Position(3, 4), Position(4, 4), Position(4, 3), Position(4, 2)
    };

    assert(cheminResident.size() == cheminResidentAttendu.size());
    assert(cheminDictionnaire.size() == cheminDictionnaireAttendu.size());
    for (std::size_t i = 0; i < cheminResident.size(); ++i) {
        assert(memePosition(cheminResident[i], cheminResidentAttendu[i]));
    }
    for (std::size_t i = 0; i < cheminDictionnaire.size(); ++i) {
        assert(memePosition(cheminDictionnaire[i], cheminDictionnaireAttendu[i]));
    }

    std::cout << "OK planification (plusCourtChemin)" << std::endl;
}


// -------------Test replanification-----------------

void testReplanification(const std::string& cheminGrilleInconnue,
                         const std::string& cheminGrilleMur) {
    const Grille grilleInconnue = chargerGrille(cheminGrilleInconnue);
    const Grille grilleMur = chargerGrille(cheminGrilleMur);

    const Position cheminDepart = grilleInconnue.getPositionDepart();
    const Position cible = grilleInconnue.getPositionResident("");
    const std::vector<Position> cheminAvecInconnue =
        plusCourtChemin(grilleInconnue, cheminDepart, cible);
    const std::vector<Position> cheminAvecMur =
        plusCourtChemin(grilleMur, grilleMur.getPositionDepart(),
                        grilleMur.getPositionResident(""));

    const std::vector<Position> cheminAvecInconnueAttendu = {
        Position(3, 3), Position(2, 3)
    };
    const std::vector<Position> cheminAvecMurAttendu = {
        Position(3, 3), Position(3, 4), Position(2, 4), Position(1, 4)
    };

    assert(memesPositions(cheminAvecInconnue, cheminAvecInconnueAttendu));
    assert(memesPositions(cheminAvecMur, cheminAvecMurAttendu));
    assert(!memesPositions(cheminAvecInconnue, cheminAvecMur));
    for (const Position& position : cheminAvecMur) {
        assert(grilleMur.getCase(position).getSymbole() != '#');
    }

    std::cout << "OK replanification (inconnue puis mur)" << std::endl;
}

// -------------Test dictionnaire-----------------

void testDictionnaire(const std::string& cheminMessages,
                      const std::string& cheminDictionnaire) {
    Dictionnaire dictionnaire(cheminDictionnaire.c_str());
    std::ifstream fichier(cheminMessages);
    assert(fichier.is_open());

    const std::vector<std::pair<std::string, std::string>> attentes = {
        {"joie", "faible"}, // content
        {"confiance", "moyenne"},   // confie
        {"peur", "faible"}, // hesitante
        {"surprise", "forte"},  // stupefaite
        {"tristesse", "forte"}, // pleurer
        {"degout", "moyenne"},  // repugnant
        {"colere", "moyenne"},  //fachee
        {"anticipation", "moyenne"},    //impatiente
        {"", ""},   // ligne vide
        {"", ""},   // Que mots inconnus
        {"anticipation", "forte"}   // guette et vide
    };

    std::string message;
    for (const auto& attendu : attentes) {
        assert(std::getline(fichier, message));
        message = Demande::enMinuscule_public(Demande::retireAccent_public(message));
        assert(dictionnaire.consulter(message) == attendu);
    }

    std::cout << "OK lecture du dictionnaire" << std::endl;
}