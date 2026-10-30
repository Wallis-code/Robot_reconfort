#pragma once

#include <string>

#include "Grille.hpp"
#include "Position.hpp"
#include "Parcours.hpp"
#include "Fouille.hpp"
#include "Trace.hpp"
#include "Demande.hpp"
#include "Dictionnaire.hpp"

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


    //Fouille de l'armoire
    Fouille fouilleCourante;       // le résultat de la fouille pour la demande en cours
    //combien d'action de CHERCHER deja fait
    //quand la fouille va nous donner le chemin de S S N E O ...
    //donc remis à 0 à chaque demande
    int prochainChercher = 0;


    //Trace
    Trace trace;
    int debutDemande = 0;          // indice du 1er pas de la demande en cours (pour pas_utilises)
    // position du sélecteur telle qu'elle doit apparaître dans la trace :
    // elle avance d'une case par CHERCHER, contrairement au vrai sélecteur (déplacé d'un coup)
    Position selecteurTrace;
 


    static Direction directionEntre(const Position& depart, const Position& arrivee) {
        if (arrivee.getX() < depart.getX()) return N;
        if (arrivee.getX() > depart.getX()) return S;
        if (arrivee.getY() > depart.getY()) return E;
        return O;
    }

    void avancerSelonChemin() {
        if (cheminActuel.size() >= 2) {
            
            Direction d = directionEntre(cheminActuel[0], cheminActuel[1]);
            ecriturePas("AVANCER", lettre(d)); //trace
            avancer(d);
        }
    }

    //fouille re

    // "tristesse" -> 4, "moyenne" -> 1
    int indiceDans(const json& liste, const std::string& valeur) {
        for (int i = 0; i < (int)liste.size(); i++)
            if (liste[i] == valeur) return i;
        return -1;
    }

    // ===================== TRACE =====================
 
    // Direction -> N S E O , en string
    std::string lettre(Direction d) {
        switch (d) {
            case N: return "N";
            case S: return "S";
            case E: return "E";
            default: return "O";
        }
    }
 
    // Symbole de la vraie grille
    std::string nomPerception(char symbole) {
        switch (symbole) {
            case '#': return "mur";
            case 'A': return "armoire";
            case 'D': return "dictionnaire";
            case 'P': return "resident";
            default:  return "libre";      // '.' et 'R'
        }
    }
 
    // Enregistre un pas dans la trace.
    // position, casier et perception sont ceux d'avant l'action. Donc c'rest a appeler avant l'action
    void ecriturePas(const std::string& action, const std::string& argument) {
        Pas p;
        p.t = (int)trace.pas.size();
        p.demande = numeroDemandeCourante;
        p.position = position;
        p.casier = selecteurTrace;
        p.action = action;
        p.argument = argument;
 
        // ce que le robot voit autour de lui, dans la vraie grille (ordre N, S, E, O)
        //c'est une fonctions du parcours mais ça marche très bien ici
        std::vector<Position> autour = voisins(position);
        //On ne peut pas faire grilleComplete.getSymboleCase(..) parceque c'est un ptr donc ->
        p.perceptionN = nomPerception(grilleComplete->getSymboleCase(autour[0]));
        p.perceptionS = nomPerception(grilleComplete->getSymboleCase(autour[1]));
        p.perceptionE = nomPerception(grilleComplete->getSymboleCase(autour[2]));
        p.perceptionO = nomPerception(grilleComplete->getSymboleCase(autour[3]));
 
        // contenu du casier sous le sélecteur, seulement si le robot est devant l'armoire
        bool devantArmoire = contient(autour, grille.getPositionArmoire());
        if (devantArmoire) {
            Casier c = armoire.getCasierCase(selecteurTrace.getX(), selecteurTrace.getY());
            p.contenuCasier = c.getObjet();
        }
 
        trace.pas.push_back(p);
    }
 
    // Enregistre la livraison de la demande en cours
    void ecritureLivraison(bool succes, const std::string& motifEchec) {
        Livraison l;
        l.demande = numeroDemandeCourante;
        l.resident = demandes[numeroDemandeCourante - 1].getIdRes();
        l.emotion = emotionCourante.first;
        l.intensite = emotionCourante.second;
        if (!fouilleCourante.objet.empty() && succes) {
            l.casierChoisi = fouilleCourante.casier;
            l.repli = repliEnTexte(fouilleCourante.repli);
            l.objet = fouilleCourante.objet;
        }
        l.pasUtilises = (int)trace.pas.size() - debutDemande;
        l.succes = succes;
        l.motifEchec = motifEchec;
        trace.livraisons.push_back(l);
    }
 
    // Fin d'une demande (réussie ou ratée) : on remet tout à zéro
    void passerDemandeSuivante() {
        objetEnMain.clear();
        emotionCourante = {"", ""};
        prochainChercher = 0;
        fouilleCourante = Fouille();
        numeroDemandeCourante++;
        debutDemande = (int)trace.pas.size();
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
        selecteurTrace = armoire.getSelecteur();    //trace

        updateGrille();
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
        //trace
        ecriturePas("DONNER", objetEnMain);
        ecritureLivraison(true, "");
        passerDemandeSuivante();
    }

    void echouer(std::string motifEchec){
        ecriturePas("ATTENDRE", "");
        ecritureLivraison(false, motifEchec);
        passerDemandeSuivante();
    }

    void consulter(){
        ecriturePas("CONSULTER", "");
        emotionCourante = dictionnaire.consulter(demandes[numeroDemandeCourante - 1].getMessage());
    }

    void prendre(){
        // 1er tour devant l'armoire
        if (prochainChercher == 0) {
            json data = armoire.getData();
            int emotion   = indiceDans(data["emotions"], emotionCourante.first);
            int intensite = indiceDans(data["intensites"],emotionCourante.second);
            //cote selecteur, tout se fait ici,
            fouilleCourante = fouiller(armoire, intensite,emotion);
        }

        // tant qu'il reste des CHERCHER : on en joue un par tour
        if (prochainChercher < (int)fouilleCourante.mouvements.size()) {
            //4 lignes de trace
            Direction d = fouilleCourante.mouvements[prochainChercher];
            ecriturePas("CHERCHER", lettre(d));                            // casier AVANT le mouvement
            selecteurTrace.deplacement(d);                                 // le sélecteur de la trace avance d'une case
            selecteurTrace.setY((selecteurTrace.getY() + 8) % 8);          // bouclage des colonnes, donc X pas concerné
            
            prochainChercher++;
            return;
        }

        // tous les CHERCHER sont faits
        if (fouilleCourante.succes) {
            ecriturePas("PRENDRE", fouilleCourante.objet);
            objetEnMain = fouilleCourante.objet;   // PRENDRE : le robot tient l'objet
            prochainChercher = 0;
        } else {                    //Armoire vide
            ecritureLivraison(false, "armoire vide");
            passerDemandeSuivante();
        }
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
                if(cheminActuel.empty()) {
                    // Si le chemin est vide, cela signifie qu'il n'y a pas de chemin vers le résident
                    echouer("pas de chemin vers le resident");
                    return;
                }
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
                    if(cheminActuel.empty()) {
                        // Si le chemin est vide, cela signifie qu'il n'y a pas de chemin vers l'armoire
                        echouer("pas de chemin vers l'armoire"); // On boucle les demandes jusqu'à ce que le robot finisse
                        return;
                    }
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
                    std::cout << "cheminActuel.size() = " << cheminActuel.size() << std::endl;
                    std::cout << "cheminActuel = ";
                    for (const Position& pos : cheminActuel) {
                        std::cout << "(" << pos.getX() << ", " << pos.getY() << ") ";
                    }
                    grille.afficher();
                    std::cout << std::endl;
                    if(cheminActuel.empty()) {
                        // Si le chemin est vide, cela signifie qu'il n'y a pas de chemin vers le dictionnaire
                        echouer("pas de chemin vers le dictionnaire"); // On boucle les demandes jusqu'à ce que le robot finisse
                        return;
                    }
                    avancerSelonChemin();
                    return;
                }
            }            
        }
    }

    const Position& getPosition() const { return position; }
    const Grille& getGrille() const { return grille; }
    const std::string& getObjetEnMain() const { return objetEnMain; }
    const Trace& getTrace() const { return trace; }
    int getNumeroDemandeCourante() const { return numeroDemandeCourante; }
    
    void setObjetEnMain(const std::string& objet) { objetEnMain = objet; }
    void setNumeroDemandeCourante(int numero) { numeroDemandeCourante = numero; }
};