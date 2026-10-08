#include "Armoire.hpp"
#include <iomanip>
#include <iostream>

Casier::Casier(std::string emotionVal, std::string intensiteVal, std::string objetVal) {
    emotion = emotionVal;
    intensite = intensiteVal;
    objet = objetVal;
}

std::string Casier::getObjet() const { return objet; }

void Casier::print() const {
    std::cout << emotion << " / " << intensite << " / " << objet << std::endl;
}

Armoire::Armoire(const char* fn) {
    data = open(fn, "robot-reconfort/armoire");
    if (data.is_null()) {
        std::cerr << "Armoire : data vide" << std::endl;
        return;
    }
    casier_depart.setX(data["casier_depart"][0]);
    casier_depart.setY(data["casier_depart"][1]);

    for (auto& c : data["casiers"]) {
        int l = c.value("ligne", -1);
        int col = c.value("colonne", -1);
        if (l < 0 || l >= 3 || col < 0 || col >= 8) continue;

        casiers[l][col] = Casier(
            c["emotion"].is_string()   ? c["emotion"].get<std::string>()   : "",
            c["intensite"].is_string() ? c["intensite"].get<std::string>() : "",
            c["objet"].is_string()     ? c["objet"].get<std::string>()     : ""
        );
    }
}

json Armoire::getData() { return data; }

Position Armoire::getCasier_depart() { return casier_depart; }

Casier Armoire::getCasierCase(int ligne, int colonne) const {
    return casiers[ligne][colonne];
}

void Armoire::printCasiers() const {
    const char* emotions[8] = {"JOIE", "CONFIANCE", "PEUR", "SURPRISE",
                               "TRISTESSE", "DEGOUT", "COLERE", "ANTICIPATION"};
    const char* intensites[3] = {"faible", "moyenne", "forte"};
    const int W = 21;

    std::cout << std::left << std::setw(10) << "";
    for (auto e : emotions) std::cout << std::setw(W) << e;
    std::cout << std::endl;

    for (int l = 0; l < 3; l++) {
        std::cout << std::setw(10) << intensites[l];
        for (int col = 0; col < 8; col++) {
            std::string o = casiers[l][col].getObjet();
            std::cout << std::setw(W) << (o.empty() ? "-" : o);
        }
        std::cout << std::endl;
    }
}