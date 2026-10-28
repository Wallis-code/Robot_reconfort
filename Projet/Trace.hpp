#pragma once
#include <string>
#include <vector>
#include "Position.hpp"

// cf. section 5.5 du sujet (pages 7-8) : format de trace.json

// Un pas = une action du robot (une ligne de "pas" dans la trace)
// La boucle du robot en crée un à chaque tour.
struct Pas {
    int t = 0;                      // numéro du tour, sur toute la simulation (pas remis à 0)
    int demande = 0;                // numéro de la demande en cours
    Position position;              // position du robot AVANT l'action
    Position casier;                // position du sélecteur de l'armoire AVANT l'action
    std::string action;             // "AVANCER", "CONSULTER", "CHERCHER", "PRENDRE", "DONNER", "ATTENDRE"
    std::string argument;           // "N", "S", "E", "O", ou le nom de l'objet ; "" = null
    std::string perceptionN;        // ce que le robot voit depuis sa position :
    std::string perceptionS;        //   "mur", "libre", "armoire", "dictionnaire", "resident"
    std::string perceptionE;
    std::string perceptionO;
    std::string contenuCasier;      // objet du casier courant si le robot est devant l'armoire ; "" = null
    std::string commentaire;        // libre ; "" = null
};

// Une livraison = le bilan d'une demande, réussie ou non
// La boucle du robot en crée une à la fin de chaque demande.
struct Livraison {
    int demande = 0;
    std::string resident;           // identifiant, ex. "R1"
    std::string emotion;            // lue dans le dictionnaire ; "" = null (emotion indeterminee)
    std::string intensite;          // "" = null
    Position casierChoisi;
    std::string repli;              // "aucun", "intensite", "voisine_1", "voisine_2" ; "" = null
    std::string objet;              // "" = null
    int pasUtilises = 0;            // nombre de pas de cette demande
    bool succes = false;
    std::string motifEchec;         // "resident inaccessible", "armoire vide", "emotion indeterminee" ; "" = null
};

// Le résumé n'est pas stocké il est calculé au moment d'écrire.
struct Trace {
    std::string carte;            
    std::string scenario;           
    std::vector<std::string> equipe;
    std::vector<Pas> pas;
    std::vector<Livraison> livraisons;

};