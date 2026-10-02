#include <fstream>
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

class JsonOpenerAbstract
{
public:
    virtual ~JsonOpenerAbstract() = 0;

    json open(const char* filename, const char* format_attendu){
        std::ifstream f(filename);           
        json data = json::parse(f);
            if((data["version"] != 1) || (data["format"] != format_attendu)){
                std::cerr << "version ou format incorrect" << std::endl;
                return NULL;
            }
        return data;
    };
};

inline JsonOpenerAbstract::~JsonOpenerAbstract() {}