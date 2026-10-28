#pragma once

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
public:
    CaseDepart() : Case('R', true) {}
    void interagir() const override {}
};

class CaseArmoire : public Case {
public:
    CaseArmoire() : Case('A', false) {}
    void interagir() const override {}
};

class CaseDictionnaire : public Case {
public:
    CaseDictionnaire() : Case('D', false) {}
    void interagir() const override {}
};

class CaseResident : public Case {
public:
    CaseResident() : Case('P', false) {}
    void interagir() const override {}
};

class CaseInconnue : public Case {
public:
    CaseInconnue() : Case('?', true) {} // On part du principe que la case inconnue est traversable
    void interagir() const override {}
};

// Factory (convertir un char du JSON en objet Case)
inline std::shared_ptr<Case> creerCase(char c) {
    switch (c) {
        case '#': return std::make_shared<CaseMur>();
        case '.': return std::make_shared<CaseLibre>();
        case 'R': return std::make_shared<CaseDepart>();
        case 'A': return std::make_shared<CaseArmoire>();
        case 'D': return std::make_shared<CaseDictionnaire>();
        case 'P': return std::make_shared<CaseResident>();
        case '?': return std::make_shared<CaseInconnue>();
        default: throw std::invalid_argument("Symbole de case inconnu");
    }
}