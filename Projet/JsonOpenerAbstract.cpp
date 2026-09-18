#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

class JsonOpenerAbstract
{
protected:
    JsonOpenerAbstract() {}

    virtual json open();
};