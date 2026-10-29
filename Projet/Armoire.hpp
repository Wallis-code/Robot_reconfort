#pragma once
#include <string>
#include "JsonOpenerAbstract.hpp"
#include "Position.hpp"

class Casier {
    std::string emotion;
    std::string intensite;
    std::string objet;
public:
    Casier() {}
    Casier(std::string emotionVal, std::string intensiteVal, std::string objetVal);
    std::string getObjet() const;
    void print() const;
};

class Armoire : public JsonOpenerAbstract {
protected:
    json data;
    Position casier_depart;
    Position selecteur;
    Casier casiers[3][8];
public:
    Armoire() {}
    Armoire(const char* fn);

    const json& getData() const;
    Position getCasier_depart();
    Position getSelecteur();
    Casier getCasierCase(int ligne, int colonne) const;
    void printCasiers() const;
    bool chercher(Direction dir);
    std::string contenuCasierCourant() const;
};