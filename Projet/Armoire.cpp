#include "JsonOpenerAbstract.cpp"
#include "Position.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>

class Casier {
    std::string emotion;
    std::string intensite;
    std::string objet;
    public:
        Casier() {};
        Casier(std::string emotionVal, std::string intensiteVal, std::string objetVal) {
        emotion = emotionVal;
        intensite = intensiteVal;
        objet = objetVal;
    }
    void print() const {
    std::cout << emotion << " / " << intensite << " / " << objet << std::endl;
    }

    std::string getObjet() const { return objet; }
};

class Armoire : public JsonOpenerAbstract {
    protected:
    json data;
    Position casier_depart;
    Casier casiers[3][8];
    public:
        //constructeur
        Armoire(){};
        Armoire(const char* fn){
            data = open(fn , "robot-reconfort/armoire");
            if(data == NULL) std::cerr << "Armoire : data vide" << std::endl;
            casier_depart.setX(data["casier_depart"][0]);
            casier_depart.setY(data["casier_depart"][1]);

            //ranger dans les casiers
            for (auto& c : data["casiers"]) {
                int l = c.value("ligne", -1);
                int col = c.value("colonne", -1);
                if (l < 0 || l >= 3 || col < 0 || col >= 8) continue;                
                casiers[l][col] = Casier(c["emotion"], 
                                        c["intensite"], 
                                        c["objet"].is_string() ? c["objet"].get<std::string>():""
                                    );
            }
        }

        //méthode
        json getData(){ return data;}
        Position getCasier_depart(){return casier_depart;}
        Casier getCasierCase(int i, int j){ return casiers[i][j]; }
};
