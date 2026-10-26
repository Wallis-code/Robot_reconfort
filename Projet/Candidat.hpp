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

struct Candidat {
    int ligne;
    int colonne;
    int distance;
    int ecart;
    bool horaire;
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
inline std::vector<Candidat> candidats(int emotion, int intensite) {
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