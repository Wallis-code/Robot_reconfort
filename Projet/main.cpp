#include "Armoire.cpp"
#include <iostream>

int main()
{
    Armoire armoire;

    json data = armoire.open("donnees/armoire_standard.json");
    json emotion = data["emotions"][5];

    std::cout << emotion << std::endl;

    return 0;
}