#pragma once
#include <algorithm>
#include <cstdlib>
#include <vector>

/*
            0 joie
   7 anticip.       1 confiance
 6 colère               2 peur
   5 dégoût         3 surprise
            4 tristesse
*/

// Un casier que le robot pourrait choisir pour répondre à une demande,
// avec ce qu'il faut pour le classer dans l'ordre de priorité (section 6.2).
struct Candidat {
    // Position du casier dans l'armoire : [ligne, colonne]
    int ligne;      // intensité du casier : 0 faible, 1 moyenne, 2 forte
    int colonne;    // émotion du casier : 0 joie ... 7 anticipation (ordre de la roue)

    // Critères de tri, comparés à la demande (intensité, émotion)
    int distance;   // écart entre l'émotion du casier et celle demandée, sur la roue (0, 1 ou 2)
    int ecart;      // écart entre l'intensité du casier et celle demandée (0, 1 ou 2)
                    //   = |ligne - intensité demandée|
                    // c'est la distance des intensités
    bool horaire;   // true si le casier est du côté des indices croissants sur la roue
                    //   (ou sur la même émotion) ; sert à départager deux casiers
                    //   à égalité : le sens horaire passe en premier
};

inline int distanceRoue(int e1, int e2) {
    int d = std::abs(e1 - e2);
    return std::min(d, 8 - d);
}

// renvoie true si a doit être essayé avant b
inline bool passeAvant(const Candidat& a, const Candidat& b) {
    if (a.distance != b.distance) return a.distance < b.distance;
    if (a.ecart != b.ecart) return a.ecart < b.ecart;
    if (a.ligne != b.ligne) return a.ligne < b.ligne;
    return a.horaire && !b.horaire;
}


//ATTENTION, ça revoie les potentiels candidats MAIS 
//on n'est pas sur que les candidats correspondent vrm à
//un objet, peut etre qu'à cet emplacement l'armoire est vide,
//d'ou l'idée d'avoir une liste
// si candidat[0] n'existe pas, on prends le [1] etc
inline std::vector<Candidat> candidats(int intensite, int emotion) {
    std::vector<Candidat> liste;
    int decalages[5] = {0, 1, -1, 2, -2};
    //MAX, voir le commentaire au début avec les émotions,
    //joie n'ira JAMAIS chercher dégout surprise et tristesse.

    for (int dec : decalages) {
        int colonne = (emotion + dec + 8) % 8;
        for (int ligne = 0; ligne < 3; ligne++) {
            Candidat c;
            c.ligne = ligne;
            c.colonne = colonne;
            c.distance = distanceRoue(emotion, colonne);
            c.ecart = std::abs(ligne - intensite);
            c.horaire = (dec >= 0);
            liste.push_back(c);
        }
    }

    std::sort(liste.begin(), liste.end(), passeAvant);
    return liste;
}