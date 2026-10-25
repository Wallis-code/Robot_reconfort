#include "JsonOpenerAbstract.cpp"
#include "Grille.hpp"
#include <fstream>
#include <iostream>


class Appartement : public JsonOpenerAbstract{
protected:
    json data;
    Grille grille;

public:
    Appartement(const char* fileName){
        data = open(fileName, "robot-reconfort/carte");
        if(data == NULL){
            std::cerr << "Format ou version de la carte appartement incorrect" << std::endl;
            return;
        }

        grille = Grille(data);
    }

    json getData(){return data;}
    Grille getGrille(){return grille;}
};