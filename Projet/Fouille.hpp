#pragma once
#include <string>
#include <vector>
#include "Armoire.hpp"
#include "Candidat.hpp"
#include "Position.hpp"


// "aucun", "intensite", "voisine_1", "voisine_2"
/*
page 10:
L'étiquette repli de la trace récapitule le résultat : 
aucun si le casier exact était garni, intensite si on est
resté sur la même émotion, voisine_1 ou voisine_2 selon la distance sur la roue
*/
enum Repli{AUCUN,INTENSITE, V1 , V2 };

// Compte rendu de la fouille, pour la boucle du robot et la trace :
// le casier choisi, l'objet, le repli, et tous les mouvements faits
struct Fouille {
    bool succes = false;                    // false = "armoire vide"
    
     // casier choisir à la fin de la fouille
     //il faudra bien penser qu'a la fin, ce casier = selecteur dans l'amoire
    Position casier;
    // l'obj choisi                       
    std::string objet;
    Repli repli;                      
    
    /*
    un peu technique, ça va être la liste des mouvements totale de la fouille
    tout les itinéraires des candidats, itinéraires est une méthode juste en dessous et candidat est 
    dans Candidat.hpp , la derniere fonction,
    */
    std::vector<Direction> mouvements;     
    
    // contenu vu après chaque mouvement
    //le robot voit ce qu'il y a dans chaque casier
    std::vector<std::string> contenusVus;   
};

//parceque la trace va attendre un string 
inline std::string repliEnTexte(Repli r) {
    switch (r) {
        case AUCUN:     return "aucun";
        case INTENSITE: return "intensite";
        case V1:        return "voisine_1";
        case V2:        return "voisine_2";
    }
    return "";
}


//pour l'algo je vais aller au plus simple, d'abord on se mets au bon endroit en hauteur
//après on gère la largeur,  avec un test pour savoir 
//si on va a droite ou a gauche (car 7+1 renvoie à 0)

/* RAPPEL
class Position {
private:    
    int x;
    int y;

            CASE N: x -= 1; break;
            case S: x += 1; break;
            case E: y += 1; break;
            case O: y -= 1; break;

Le pdf dit ça (page 18)
verticalement, |l' − l| mouvements, S si l' > l , N sinon ;
horizontalement, on pose a = (c' − c) mod 8 . Si a ≤ 8 − a , ce sont a mouvements E ; sinon ce
sont 8 − a mouvements O 
*/

inline std::vector<Direction> itineraire(Position depart, Position arrivee){
    std::vector<Direction> mouvements;
    int dx = depart.getX(), dy = depart.getY();
    int ax = arrivee.getX(), ay = arrivee.getY();

    // vertical : les lignes ne bouclent pas
    //idéalement je peux peut-etre faire un test
    //pour savoir s'il faut descendre ou monter mais 
    //cette facon fonctionne car une des deux boucles ne se fera pas
    //et c'est pas problématique
    for (int i = dx; i < ax; i++) mouvements.push_back(S);   // descendre
    for (int i = dx; i > ax; i--) mouvements.push_back(N);   // monter

    // horizontal : arrivé - depart 
    //Jamais négatif, le mod % en c++ garde le négatif, contrairement en maths
    // -2 % 8 = -2 alors qur'en maths c'est 6
    //donc je rajoute + 8 pour avoir un modulo comme en maths classique
    //et faire exactement ce qui est mis dans l'énoncé (voir au dessus de la signature)
    int a = (ay - dy + 8) % 8;
    if (a <= 8 - a)
        for (int i = 0; i < a; i++) mouvements.push_back(E);
    else
        for (int i = 0; i < 8 - a; i++) mouvements.push_back(O);   // plus court par l'ouest, ça boucle

    return mouvements;
}

// Étiquette "repli" de la trace, à partir du candidat choisi (section 6.2)
inline Repli calculRepli(const Candidat& c) {
    if (c.distance == 0 && c.ecart == 0) return AUCUN;
    if (c.distance == 0) return INTENSITE;
    if (c.distance == 1) return V1;
    return V2 ;
}
 
// Le robot fouille l'armoire : il essaie les candidats dans l'ordre,
// déplace le sélecteur casier par casier, et prend le premier garni
inline Fouille fouiller(Armoire& armoire, int intensite, int emotion) {
    Fouille f;
 
    for (const Candidat& c : candidats(intensite, emotion)) {
        Position cible(c.ligne, c.colonne);
 
        // aller jusqu'au candidat
        for (Direction d : itineraire(armoire.getSelecteur(), cible)) {
            //CA DEPLACE LE SELECTEUR
            armoire.chercher(d);
            f.mouvements.push_back(d);
            f.contenusVus.push_back(armoire.contenuCasierCourant());
        }
 
        // arrivé sur le candidat : pas vide -> on le prend, vide -> candidat suivant
        std::string contenu = armoire.contenuCasierCourant();
        if (!contenu.empty()) {
            f.succes = true;
            f.casier = cible;
            f.objet = contenu;
            f.repli = calculRepli(c);
            return f;
        }
    }
 
    // les 15 candidats sont vide -> l'amoire est renvoyé comme vide
    //mais au final on ne sait pas vraiment car il est possible que juste ces 15 soient vides
    return f;   
}
