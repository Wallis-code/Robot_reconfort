#pragma once
#include <iostream>
#include "Position.hpp"

class Grille {
protected:    
    char** cases;
    int[2] dimensions;
    
public:        
    //constructeurs
    Grille() : cases(NULL) {}
    Grille(json data){
        
        std::string grille_str;        
        std::string dim_str;

        if((dim_str = data["dimensions"].dump()) == "null"){
            std::cerr << "L'appartement ne contient pas de dimensions." << std::endl;
            return;
        }

        int index_hauteur = dim_str.find("hauteur");
        int index_largeur = dim_str.find("largeur");
        if(index_hauteur > 30 || index_largeur > 30){
            std::cerr << "Les dimensions n'ont pas le bon format" << std::endl;
            return;
        }

        std::string nbr_str;
        int curr = index_hauteur;
        while(dim_str[curr] != ','){
            if(dim_str[curr] > 48 && dim_str[curr] < 57){ // Si chiffre
                nbr_str.push_back
            }
        }


        
        if((grille_str = data["grille"].dump()) == "null"){
            std::cerr << "L'appartement ne contient pas de grille." << std::endl;
            return;
        }
        
    }
    
    //getter setter
    char getCase(Position pos) const { return cases[pos.getX()][pos.getY()];}
    void setY(Position pos, char symbole){ cases[pos.getX()][pos.getY()] = symbole;}

};