#include "JsonOpenerAbstract.cpp"
#include "Position.hpp"
#include <fstream>
#include <iostream>

class Armoire : public JsonOpenerAbstract {
    protected:
    json data;
    Position casier_depart;
    public:
        //constructeur
        Armoire(){};
        Armoire(const char* fn){
            data = open(fn , "robot-reconfort/armoire");
            if(data == NULL) std::cerr << "Armoire : data vide" << std::endl;
            casier_depart.setX(data["casier_depart"][0]);
            casier_depart.setY(data["casier_depart"][1]);
        }

        //méthode
        json getData(){ return data;}
        Position getCasier_depart(){return casier_depart;}
};
