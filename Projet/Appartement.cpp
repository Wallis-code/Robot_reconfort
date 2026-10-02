#include "JsonOpenerAbstract.cpp"
#include <fstream>
#include <iostream>


class Appartement : public JsonOpenerAbstract{
protected:
    json data;
    int dimensions[2];
    Grille grille;

public:
    Appartement(const char* fileName){
        data = open(fileName, "robot-reconfort/carte");
        if(data == NULL){
            std::cerr << "Format ou version de la carte appartement incorrect" << std::endl;
            return;
        }
    }
};