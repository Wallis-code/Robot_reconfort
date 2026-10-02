#include "JsonOpenerAbstract.cpp"
#include "Position.hpp"
#include <fstream>
#include <iostream>

class Armoire : public JsonOpenerAbstract {
public:
        json open(const char* fn){
            std::ifstream f(fn);           
            json data = json::parse(f);
            if((data["version"] != 1) || (data["format"] != "robot-reconfort/armoire")){
                std::cerr << "Armoire version ou format incorrect" << std::endl;
                return NULL;
            }
            return data;
        }


};
