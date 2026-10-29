#pragma once
#include <queue>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "Position.hpp"
#include "Grille.hpp"

using json = nlohmann::json;

inline bool memePosition(Position a, Position b) {
    return a.getX() == b.getX() && a.getY() == b.getY();
}

inline bool contient(const std::vector<Position>& liste, Position p) {
    for (const Position& q : liste)
        if (memePosition(q, p)) return true;
    return false;
}

// Les 4 cases autour d'une position : N, S, E, O
inline std::vector<Position> voisins(Position p) {
    return {
        Position(p.getX() - 1, p.getY()),
        Position(p.getX() + 1, p.getY()),
        Position(p.getX(), p.getY() + 1),
        Position(p.getX(), p.getY() - 1)
    };
}


//Parcours en largeur, cf. annexe B.4 (page 16)
//À lancer UNIQUEMENT sur la carte mentale du robot, jamais sur la vraie grille (section 3.2).
//Renvoie le chemin [depart, ..., arrivee] vers l'arrivée la plus proche,
//ou un vecteur vide s'il n'existe aucun chemin.
//Plusieurs arrivées possibles : par exemple toutes les cases libres autour de l'armoire.
//LA FONCTION CALCUL ELLE MEME LES 4 POSSIBLES POSITIONS AUTOUR
//ALORS ON METS DIRECTEMENT LA POSITION DE L'ARMOIRE OU DU RESIDENT 
inline std::vector<Position> plusCourtChemin(const Grille& carte, Position depart, Position cible) {
    std::vector<Position> arrivees = voisins(cible);
    if (contient(arrivees, depart)) return {depart};   // déjà à côté de la cible
 
    int H = carte.getHauteur();
    int L = carte.getLargeur();
 
    // Tableau 2D de la meme taille que la grille qui servira à savoir 
    //si une case de la grille a déjà été éxploré ou pas
    std::vector<std::vector<bool>> vu(H, std::vector<bool>(L, false));
    
    // Tableau 2D de la même taille que la grille pour chaque case découverte,
    // la case d'où on est arrivé dessus (son prédécesseur).
    std::vector<std::vector<Position>> predecesseur(H, std::vector<Position>(L));
 
    std::queue<Position> file;   // la file pour savoir quelles cases piocher
    file.push(depart);
    vu[depart.getX()][depart.getY()] = true;
 
    // voisins dans l'ordre N, S, E, O (X = ligne, Y = colonne)
    int dl[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
 
    while (!file.empty()) {
        Position courant = file.front(); //la plus ancienne
        file.pop(); //on la pop vu que la traite et qu'elle est stocké dans courant
 
        //on va faire une boucle de 4 pour les 4 voisines
        for (int k = 0; k < 4; k++) {
            int l = courant.getX() + dl[k];
            int c = courant.getY() + dc[k];
            Position voisin(l, c);
 
            if (l < 0 || l >= H || c < 0 || c >= L) continue;        // hors de la grille
            if (vu[l][c]) continue;                                  // déjà vue
            if (!carte.getCase(voisin).estTraversable()) continue;   // on ne peut pas passer
 
            //Si on n'a pas deja vu,dans la grille, et traversable :
            vu[l][c] = true;
            predecesseur[l][c] = courant; // on retient qu'on y est arrivé depuis "courant"
 
            // Est-ce une des cases à côté de la cible ? Si oui, c'est fini :
            if (contient(arrivees, voisin)) {
                // On reconstruit le chemin en remontant les prédécesseurs,
                // de l'arrivée jusqu'au départ (il sera donc à l'envers)
                std::vector<Position> chemin;
                Position p = voisin;
                while (!memePosition(p, depart)) {
                    chemin.push_back(p);
                    p = predecesseur[p.getX()][p.getY()];   // on recule d'une case
                }
                chemin.push_back(depart);     // le départ n'a pas de prédécesseur, on l'ajoute à la main

                // On remet le chemin dans le bon sens
                std::reverse(chemin.begin(), chemin.end());
                return chemin;
            }

            // Pas encore arrivé : on met la case dans la file,
            // ses propres voisins seront explorés plus tard
            file.push(voisin);
        }
    }

    // La file est vide et on n'a jamais atteint la cible : aucun chemin n'existe
    // (sur la carte mentale, donc pas non plus dans la réalité, cf. annexe B.6)
    return {};
}