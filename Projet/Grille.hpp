#pragma once
#include <iostream>
#include "Position.hpp"

class Grille {
protected:    
    char** cases;
    
public:        
    //constructeurs
    Grille() : cases(NULL) {}
    
    //getter setter
    char getCase(Position pos) const { return cases[pos.getX()][pos.getY()];}
    void setY(Position pos, char symbole){ cases[pos.getX()][pos.getY()] = symbole;}

};