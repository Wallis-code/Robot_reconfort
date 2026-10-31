#pragma once

#include <memory>
#include <string>

#include "Position.hpp"

class Case {
protected:
    char symbole;
    bool traversable;

public:
    virtual ~Case() = default;
    Case(char s, bool t) : symbole(s), traversable(t) {}

    char getSymbole() const { return symbole; }
    bool estTraversable() const { return traversable; }

    virtual void interagir() const = 0; 
};


// --- Sous-classes ---

class CaseMur : public Case {
public:
    CaseMur() : Case('#', false) {}
    void interagir() const override {}
};

class CaseLibre : public Case {
public:
    CaseLibre() : Case('.', true) {}
    void interagir() const override {}
};

class CaseDepart : public Case {
private:
    Position position;

public:
    CaseDepart(const Position& pos = Position()) : Case('R', true), position(pos) {}

    const Position& getPosition() const { return position; }
    void interagir() const override {}
};

class CaseArmoire : public Case {
private:
    Position position;

public:
    CaseArmoire(const Position& pos = Position()) : Case('A', false), position(pos) {}

    const Position& getPosition() const { return position; }
    void interagir() const override {}
};

class CaseDictionnaire : public Case {
private:
    Position position;

public:
    CaseDictionnaire(const Position& pos = Position()) : Case('D', false), position(pos) {}

    const Position& getPosition() const { return position; }
    void interagir() const override {}
};

class CaseResident : public Case {
private:
    std::string id;
    std::string nom;
    Position position;

public:
    CaseResident(const std::string& idVal = "", const std::string& nomVal = "", const Position& pos = Position())
        : Case('P', false), id(idVal), nom(nomVal), position(pos) {}

    const std::string& getId() const { return id; }
    const std::string& getNom() const { return nom; }
    const Position& getPosition() const { return position; }
    void interagir() const override {}
};

class CaseInconnue : public Case {
public:
    CaseInconnue() : Case('?', true) {} // On part du principe que la case inconnue est traversable
    void interagir() const override {}
};

// Factory (convertir un char du JSON en objet Case)
inline std::shared_ptr<Case> creerCase(char c, const Position& pos = Position(),
                                      const std::string& id = "", const std::string& nom = "") {
    switch (c) {
        case '#': return std::make_shared<CaseMur>();
        case '.': return std::make_shared<CaseLibre>();
        case 'R': return std::make_shared<CaseDepart>(pos);
        case 'A': return std::make_shared<CaseArmoire>(pos);
        case 'D': return std::make_shared<CaseDictionnaire>(pos);
        case 'P': return std::make_shared<CaseResident>(id, nom, pos);
        case '?': return std::make_shared<CaseInconnue>();
        default: throw std::invalid_argument("Symbole de case inconnu");
    }
}