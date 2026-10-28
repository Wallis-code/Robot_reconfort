#pragma once

#include <string>

#include "Grille.hpp"
#include "Position.hpp"
#include "Parcours.hpp"

class Robot {
private:
    Position position;
    Grille grille;
    const Grille* grilleComplete;
    std::string objetEnMain;
    int numeroDemandeCourante;
    int dimensions[2];

public:
    Robot() : position(), grille(), grilleComplete(nullptr), objetEnMain(""), numeroDemandeCourante(0) {
        dimensions[0] = 0;
        dimensions[1] = 0;
    }

    Robot(const Grille& grilleVal, const std::string& objetVal = "", int numeroDemandeVal = 0)
        : grille(grilleVal), grilleComplete(&grilleVal), objetEnMain(objetVal), numeroDemandeCourante(numeroDemandeVal) {
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

    void updateGrille(){
        std::vector<Position> casesAdj = voisins(position);
        for (const Position& v : casesAdj) {
            const char symboleConnu = grilleComplete->getSymboleCase(v);
            const char symboleVision = grille.getSymboleCase(v);

            if (symboleVision == '?' && symboleConnu != '?') {
                grille.setCase(v, creerCase(symboleConnu, v));
            }
        }
    }

    void avancer(Direction dir){
        position.deplacement(dir);
        updateGrille();
    }

    const Position& getPosition() const { return position; }
    const Grille& getGrille() const { return grille; }
    const std::string& getObjetEnMain() const { return objetEnMain; }
    int getNumeroDemandeCourante() const { return numeroDemandeCourante; }
    
    void setObjetEnMain(const std::string& objet) { objetEnMain = objet; }
    void setNumeroDemandeCourante(int numero) { numeroDemandeCourante = numero; }
};
