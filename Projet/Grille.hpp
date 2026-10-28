#pragma once
#include <iostream>
#include "Position.hpp"
#include "Case.hpp"
#include <vector>

class Grille {
protected:    
    std::vector<std::vector<std::shared_ptr<Case>>> cases; // shared ptr pour pouvoir copier/modifier les cases facilement
    int dimensions[2];
    
public:        
    //constructeurs
    Grille() : dimensions{0, 0} {}

    Grille(json data){

        if(!data.contains("dimensions") || !data["dimensions"].is_object()){
            std::cerr << "L'appartement ne contient pas de dimensions." << std::endl;
            return;
        }

        const auto& dim = data["dimensions"];
        if (!dim.contains("hauteur") || !dim["hauteur"].is_number_integer() ||
            !dim.contains("largeur") || !dim["largeur"].is_number_integer()) {
            std::cerr << "Erreur : Les dimensions 'hauteur' et 'largeur' sont manquantes ou invalides." << std::endl;
            return;
        }

        int hauteur = dim["hauteur"].get<int>();
        int largeur = dim["largeur"].get<int>();

        if (hauteur <= 0 || largeur <= 0) {
            std::cerr << "Les dimensions doivent être supérieurs à 0." << std::endl;
            return;
        }

        if (!data.contains("grille") || !data["grille"].is_array()) {
            std::cerr << "L'appartement ne contient pas de grille valide." << std::endl;
            return;
        }

        std::vector<std::string> lignes = data["grille"].get<std::vector<std::string>>();

        if (static_cast<int>(lignes.size()) != hauteur) {
            std::cerr << "La hauteur de la grille ne correspond pas." << std::endl;
            return;
        }

        for (int i = 0; i < hauteur; ++i) {
            if (static_cast<int>(lignes[i].size()) != largeur) {
                std::cerr << "La largeur de la ligne " << i << " ne correspond pas" << std::endl;
                return;
            }
        }

        dimensions[0] = hauteur;
        dimensions[1] = largeur;

        cases.resize(hauteur);
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                cases[i].push_back(creerCase(lignes[i][j], Position(i, j)));
            }
        }

        if (data.contains("residents") && data["residents"].is_array()) {
            for (const auto& resident : data["residents"]) {
                if (!resident.contains("position") || !resident["position"].is_array()) {
                    continue;
                }

                const auto& posJson = resident["position"];
                const Position pos(posJson[0].get<int>(), posJson[1].get<int>());

                if (pos.getX() >= 0 && pos.getX() < hauteur &&
                    pos.getY() >= 0 && pos.getY() < largeur) {
                    cases[pos.getX()][pos.getY()] = std::make_shared<CaseResident>(
                        resident["id"].get<std::string>(),
                        resident["nom"].get<std::string>(),
                        pos
                    );
                }
            }
        }
    }

    void afficher() const {
        for (const auto& ligne : cases) {
            for (const auto& casePtr : ligne) {
                std::cout << casePtr->getSymbole();
            }
            std::cout << std::endl;
        }
    }
    
    //getter setter
    const Case& getCase(Position pos) const { 
        return *cases[pos.getX()][pos.getY()]; 
    }
    char getSymboleCase(Position pos) const { 
        return cases[pos.getX()][pos.getY()]->getSymbole(); 
    }
    void setCase(const Position& pos, const std::shared_ptr<Case>& nouvelleCase) {
        cases[pos.getX()][pos.getY()] = nouvelleCase;
    }
    void setY(Position pos, char symbole) { 
        cases[pos.getX()][pos.getY()] = creerCase(symbole); 
    }
    int getHauteur() const { return dimensions[0]; }
    int getLargeur() const { return dimensions[1]; }


    // Plus propre : a refaire en une seule fonction

    Position getPositionDepart() const {
        for (int i = 0; i < dimensions[0]; ++i) {
            for (int j = 0; j < dimensions[1]; ++j) {
                const auto& objet = cases[i][j];
                if (objet && objet->getSymbole() == 'R') {
                    return Position(i, j);
                }
            }
        }
        return Position();
    }

    Position getPositionArmoire() const {
        for (int i = 0; i < dimensions[0]; ++i) {
            for (int j = 0; j < dimensions[1]; ++j) {
                const auto& objet = cases[i][j];
                if (objet && objet->getSymbole() == 'A') {
                    return Position(i, j);
                }
            }
        }
        return Position();
    }

    Position getPositionDictionnaire() const {
        for (int i = 0; i < dimensions[0]; ++i) {
            for (int j = 0; j < dimensions[1]; ++j) {
                const auto& objet = cases[i][j];
                if (objet && objet->getSymbole() == 'D') {
                    return Position(i, j);
                }
            }
        }
        return Position();
    }

    Position getPositionResident(const std::string& id) const {
        for (int i = 0; i < dimensions[0]; ++i) {
            for (int j = 0; j < dimensions[1]; ++j) {
                const auto& objet = cases[i][j];
                if (objet && objet->getSymbole() == 'P') {
                    const auto resident = std::dynamic_pointer_cast<const CaseResident>(objet);
                    if (resident && resident->getId() == id) {
                        return Position(i, j);
                    }
                }
            }
        }
        return Position();
    }

};