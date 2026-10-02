#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

class JsonOpenerAbstract
{
public:
    virtual json open(const char* filename) = 0;
};