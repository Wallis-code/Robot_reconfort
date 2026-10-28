#pragma once

#include <string>

#include "Grille.hpp"
#include "Position.hpp"
#include "Parcours.hpp"
#include "Fouille.hpp"

class Robot {
private:
    Position position;

    Grille grille;
    const Grille* grilleComplete;
    int dimensions[2];

    std::vector<Position> cheminActuel;

    std::vector<Demande> demandes;
    int numeroDemandeCourante;
    
    std::pair<std::string, std::string> emotionCourante; // {emotion, intensite}
    std::string objetEnMain;

    Dictionnaire dictionnaire;

    Armoire armoire;

    static Direction directionEntre(const Position& depart, const Position& arrivee) {
        if (arrivee.getX() < depart.getX()) return N;
        if (arrivee.getX() > depart.getX()) return S;
        if (arrivee.getY() > depart.getY()) return E;
        return O;
    }

    void avancerSelonChemin() {
        if (cheminActuel.size() >= 2) {
            avancer(directionEntre(cheminActuel[0], cheminActuel[1]));
        }
    }

public:
    Robot() : position(), grille(), grilleComplete(nullptr), dimensions{0, 0},
        cheminActuel(), demandes(), numeroDemandeCourante(0), emotionCourante(),
        objetEnMain(), dictionnaire(), armoire() {
    }

    Robot(const Grille& grilleVal, const std::vector<Demande>& demandesVal,
          const Dictionnaire& dictionnaireVal, const Armoire& armoireVal)
        : position(), grille(grilleVal), grilleComplete(&grilleVal), dimensions{0, 0},
        cheminActuel(), demandes(demandesVal), numeroDemandeCourante(1), emotionCourante(),
        objetEnMain(), dictionnaire(dictionnaireVal), armoire(armoireVal) {
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

    // prerequis : la case dans la direction dir est traversable
    void avancer(Direction dir){
        grille.setY(position, '.'); // on efface le robot de sa position actuelle
        position.deplacement(dir);
        grille.setY(position, 'R'); // on met le robot à sa nouvelle position
        updateGrille();
    }

    void donner(){
        objetEnMain.clear();
        numeroDemandeCourante++;
    }

    void consulter(){
        emotionCourante = dictionnaire.consulter(demandes[numeroDemandeCourante - 1].getMessage());
    }

    // TODO
    void prendre(){
        //objetEnMain = armoire.prendre();
    }

    void choixAction(){
        bool tientObjet = !objetEnMain.empty();
        std::vector<Position> casesAdj = voisins(position);

        if(tientObjet){
            // Si le robot tient un objet:
            bool aCoteResident = contient(casesAdj, grille.getPositionResident(demandes[numeroDemandeCourante - 1].getIdRes()));
            if(aCoteResident){  // Si le robot est à côté du résident
                donner();
                return;
            } else {    // Si le robot n'est pas à côté du résident
                cheminActuel = plusCourtChemin(grille, position, grille.getPositionResident(demandes[numeroDemandeCourante - 1].getIdRes()));
                avancerSelonChemin();
                return;
            }
        } else { // Si le robot ne tient pas d'objet:
            bool emotionDeterminee = !emotionCourante.first.empty();
            if(emotionDeterminee){ // Si l'émotion est déterminée
                bool aCoteArmoire = contient(casesAdj, grille.getPositionArmoire());
                if(aCoteArmoire){   // Si le robot est à côté de l'armoire
                    prendre();
                    return;
                } else {    // Si le robot n'est pas à côté de l'armoire
                    cheminActuel = plusCourtChemin(grille, position, grille.getPositionArmoire());
                    avancerSelonChemin();
                    return;
                }
            } else {  // Si l'émotion n'est pas déterminée
                bool aCoteDictionnaire = contient(casesAdj, grille.getPositionDictionnaire());
                if(aCoteDictionnaire){   // Si le robot est à côté du dictionnaire
                    consulter();
                    return;
                } else {    // Si le robot n'est pas à côté du dictionnaire
                    cheminActuel = plusCourtChemin(grille, position, grille.getPositionDictionnaire());
                    avancerSelonChemin();
                    return;
                }
            }            
        }
    }

    const Position& getPosition() const { return position; }
    const Grille& getGrille() const { return grille; }
    const std::string& getObjetEnMain() const { return objetEnMain; }
    int getNumeroDemandeCourante() const { return numeroDemandeCourante; }
    
    void setObjetEnMain(const std::string& objet) { objetEnMain = objet; }
    void setNumeroDemandeCourante(int numero) { numeroDemandeCourante = numero; }
};
