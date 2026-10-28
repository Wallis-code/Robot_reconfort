#pragma once

#include <string>

#include "Grille.hpp"
#include "Position.hpp"

class Robot {
private:
    Position position;
    Grille grille;
    std::string objetEnMain;
    int numeroDemandeCourante;
    int dimensions[2];

public:
    Robot() : position(), grille(), objetEnMain(""), numeroDemandeCourante(0) {}

    Robot(const Grille& grilleVal, const std::string& objetVal = "", int numeroDemandeVal = 0)
        : objetEnMain(objetVal), numeroDemandeCourante(numeroDemandeVal) {
        grille = grilleVal;
        dimensions[0] = grille.getHauteur();
        dimensions[1] = grille.getLargeur();

        // On remplace les cases libres et les murs dans l'appart par des cases inconnues
        for (int x = 0; x < dimensions[0]; ++x) {
            for (int y = 0; y < dimensions[1]; ++y) {
                const Position pos(x, y);
                const bool estBord = (x == 0 || y == 0 || x == dimensions[0] - 1 || y == dimensions[1] - 1);
                const char symbole = grille.getSymboleCase(pos);

                if (!estBord && (symbole == '.' || symbole == '#')) {
                    grille.setCase(pos, std::make_shared<CaseInconnue>());
                }
            }
        }

        position = grille.getPositionDepart();
    }

    const Position& getPosition() const { return position; }
    const Grille& getGrille() const { return grille; }
    const std::string& getObjetEnMain() const { return objetEnMain; }
    int getNumeroDemandeCourante() const { return numeroDemandeCourante; }
    
    void setObjetEnMain(const std::string& objet) { objetEnMain = objet; }
    void setNumeroDemandeCourante(int numero) { numeroDemandeCourante = numero; }
};
