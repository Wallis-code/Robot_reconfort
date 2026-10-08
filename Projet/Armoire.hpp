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
    Casier casiers[3][8];
public:
    Armoire() {}
    Armoire(const char* fn);

    json getData();
    Position getCasier_depart();
    Casier getCasierCase(int ligne, int colonne) const;
    void printCasiers() const;
};