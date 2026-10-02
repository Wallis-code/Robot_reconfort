#include "JsonOpenerAbstract.cpp"
#include <fstream>
#include <iostream>


class Appartement : public JsonOpenerAbstract{
public:
        json open(const char* fileName){
            std::ifstream f(fileName);
            json data = json::parse(f);  
            if((data["format"] != "robot-reconfort/carte") || (data["version"] != 1)){
                std::cerr << "Format ou version de la carte appartement incorrect" << std::endl;
                return NULL;
            }                      
            return data;
        }
};