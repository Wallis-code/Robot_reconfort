#pragma once
#include <iostream>
#include "Position.hpp"
#include "Case.hpp"
#include <vector>

class Grille {
protected:    
    std::vector<std::vector<std::shared_ptr<Case>>> cases; // shared ptr pour pouvoir copier/modifier les cases facilement
    int dimensions[2];
    
public:        
    //constructeurs
    Grille() : dimensions{0, 0} {}

    Grille(json data){

        if(!data.contains("dimensions") || !data["dimensions"].is_object()){
            std::cerr << "L'appartement ne contient pas de dimensions." << std::endl;
            return;
        }

        const auto& dim = data["dimensions"];
        if (!dim.contains("hauteur") || !dim["hauteur"].is_number_integer() ||
            !dim.contains("largeur") || !dim["largeur"].is_number_integer()) {
            std::cerr << "Erreur : Les dimensions 'hauteur' et 'largeur' sont manquantes ou invalides." << std::endl;
            return;
        }

        int hauteur = dim["hauteur"].get<int>();
        int largeur = dim["largeur"].get<int>();

        if (hauteur <= 0 || largeur <= 0) {
            std::cerr << "Les dimensions doivent être supérieurs à 0." << std::endl;
            return;
        }

        if (!data.contains("grille") || !data["grille"].is_array()) {
            std::cerr << "L'appartement ne contient pas de grille valide." << std::endl;
            return;
        }

        std::vector<std::string> lignes = data["grille"].get<std::vector<std::string>>();

        if (static_cast<int>(lignes.size()) != hauteur) {
            std::cerr << "La hauteur de la grille ne correspond pas." << std::endl;
            return;
        }

        for (int i = 0; i < hauteur; ++i) {
            if (static_cast<int>(lignes[i].size()) != largeur) {
                std::cerr << "La largeur de la ligne " << i << " ne correspond pas" << std::endl;
                return;
            }
        }

        dimensions[0] = hauteur;
        dimensions[1] = largeur;

        cases.resize(hauteur);
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                cases[i].push_back(creerCase(lignes[i][j]));
    }
}
    }
    
    //getter setter
    const Case& getCase(Position pos) const { 
        return *cases[pos.getX()][pos.getY()]; 
    }
    char getSymboleCase(Position pos) const { 
        return cases[pos.getX()][pos.getY()]->getSymbole(); 
    }
    void setY(Position pos, char symbole) { 
        cases[pos.getX()][pos.getY()] = creerCase(symbole); 
    }
    int getHauteur() const { return dimensions[0]; }
    int getLargeur() const { return dimensions[1]; }

};