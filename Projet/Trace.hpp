#pragma once
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "json.hpp"
#include "Position.hpp"
 
using json = nlohmann::json;

// cf. section 5.5 du sujet (pages 7-8) : format de trace.json

// Un pas = une action du robot (une ligne de "pas" dans la trace)
// La boucle du robot en crée un à chaque tour.
struct Pas {
    int t = 0;                      // numéro du tour, sur toute la simulation (pas remis à 0)
    int demande = 0;                // numéro de la demande en cours
    Position position;              // position du robot AVANT l'action
    Position casier;                // position du sélecteur de l'armoire AVANT l'action
    std::string action;             // "AVANCER", "CONSULTER", "CHERCHER", "PRENDRE", "DONNER", "ATTENDRE"
    std::string argument;           // "N", "S", "E", "O", ou le nom de l'objet ; "" = null
    std::string perceptionN;        // ce que le robot voit depuis sa position :
    std::string perceptionS;        //   "mur", "libre", "armoire", "dictionnaire", "resident"
    std::string perceptionE;
    std::string perceptionO;
    std::string contenuCasier;      // objet du casier courant si le robot est devant l'armoire ; "" = null
    std::string commentaire;        // libre ; "" = null
};

// Une livraison = le bilan d'une demande, réussie ou non
// La boucle du robot en crée une à la fin de chaque demande.
struct Livraison {
    int demande = 0;
    std::string resident;           // identifiant, ex. "R1"
    std::string emotion;            // lue dans le dictionnaire ; "" = null (emotion indeterminee)
    std::string intensite;          // "" = null
    Position casierChoisi;
    std::string repli;              // "aucun", "intensite", "voisine_1", "voisine_2" ; "" = null
    std::string objet;              // "" = null
    int pasUtilises = 0;            // nombre de pas de cette demande
    bool succes = false;
    std::string motifEchec;         // "resident inaccessible", "armoire vide", "emotion indeterminee" ; "" = null
};

// Le résumé n'est pas stocké il est calculé au moment d'écrire.
struct Trace {
    std::string carte;            
    std::string scenario;           
    std::vector<std::string> equipe;
    std::vector<Pas> pas;
    std::vector<Livraison> livraisons;

};


// "" dans nos structs veut dire null dans le json
inline json texteOuNull(const std::string& s) {
    if (s.empty()) return nullptr;
    return s;
}
 
// Position -> [ligne, colonne]
inline json positionJson(Position p) {
    return json::array({p.getX(), p.getY()});
}
 
//met un pas en json
inline json pasJson(const Pas& p) {
    json j;
    j["t"] = p.t;
    j["demande"] = p.demande;
    j["position"] = positionJson(p.position);
    j["casier"] = positionJson(p.casier);
    j["action"] = p.action;
    j["argument"] = texteOuNull(p.argument);
    j["perception"] = {
        {"N", p.perceptionN},
        {"S", p.perceptionS},
        {"E", p.perceptionE},
        {"O", p.perceptionO}
    };
    j["contenu_casier"] = texteOuNull(p.contenuCasier);
    j["commentaire"] = texteOuNull(p.commentaire);
    return j;
}
 
//Mets une livraison en Json
inline json livraisonJson(const Livraison& l) {
    json j;
    j["demande"] = l.demande;
    j["resident"] = texteOuNull(l.resident);
    j["emotion"] = texteOuNull(l.emotion);
    j["intensite"] = texteOuNull(l.intensite);
    if (l.objet.empty()) j["casier_choisi"] = nullptr;
        else j["casier_choisi"] = positionJson(l.casierChoisi);
    j["repli"] = texteOuNull(l.repli);
    j["objet"] = texteOuNull(l.objet);
    j["pas_utilises"] = l.pasUtilises;
    j["succes"] = l.succes;
    j["motif_echec"] = texteOuNull(l.motifEchec);
    return j;
}
 
// Écrit trace.json, résumé compris. À n'appeler qu'à la toute fin :
// en cas d'erreur de données, aucune trace ne doit être écrite.
inline void ecrireTrace(const Trace& trace, const std::string& chemin) {
    json data;
 
    // début
    data["format"] = "robot-reconfort/trace";
    data["version"] = 1;
    data["carte"] = trace.carte;
    data["scenario"] = trace.scenario;
    data["equipe"] = trace.equipe;
 
    //cette ligne signifie qu'on va créer une liste dans le Json
    data["pas"] = json::array();
    for (const Pas& p : trace.pas)
        data["pas"].push_back(pasJson(p));
 
    // un objet par demande
    data["livraisons"] = json::array();
    int reussies = 0;
    for (const Livraison& l : trace.livraisons) {
        data["livraisons"].push_back(livraisonJson(l));
        if (l.succes) reussies++;
    }
 
    // le résumé est calculé ici
    int demandes = (int)trace.livraisons.size();
    data["resume"] = {
        {"demandes", demandes},
        {"reussies", reussies},
        {"echecs", demandes - reussies},
        {"pas_total", (int)trace.pas.size()}
    };
 
    std::ofstream f(chemin);
    if (!f) throw std::runtime_error("Impossible d'écrire la trace : " + chemin);
    f << data.dump(2) << std::endl;
}
