#pragma once
#include <iostream>

//Nord, Sud, Est, Ouest (west en anglais)
enum Direction { N, S, E, W };

class Position {
private:    
    int x;
    int y;
    
public:        
    //constructeurs
    Position() : x(0), y(0) {}
    Position(int xVal, int yVal) : x(xVal), y(yVal) {}
    
    //getter setter
    int getX() const { return x;}
    int getY() const { return y;}
    void setX(int newX){ x = newX;}
    void setY(int newY){ y = newY;}

    //Méthode

    void deplacement(Direction d){
        switch(d){
            case N:
                y += 1;
                break;
            case S:
                y -= 1;
                break;
            case E:
                x += 1;
                break;
            case W:
                x -= 1;
                break;
            default:
                break;
        }
    }
};

inline std::ostream& operator<<(std::ostream& os, const Position& v) {
    return os << "(" << v.getX() << ", " << v.getY() << ")";
}