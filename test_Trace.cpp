#include <cassert>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "Trace.hpp"

static const char* FICHIER = "test/test_trace.json";

// Une trace inventée à la main : 2 pas, 1 réussite, 1 échec
static Trace traceExemple() {
    Trace trace;
    trace.carte = "appartement_01";
    trace.scenario = "scenario_01";
    trace.equipe = {"MOREAU Wallis", "PRATVIEL Sebastien"};

    Pas p1;
    p1.t = 0; p1.demande = 1;
    p1.position = Position(6, 1); p1.casier = Position(1, 0);
    p1.action = "AVANCER"; p1.argument = "E";
    p1.perceptionN = "libre"; p1.perceptionS = "libre";
    p1.perceptionE = "libre"; p1.perceptionO = "mur";
    trace.pas.push_back(p1);

    Pas p2;
    p2.t = 1; p2.demande = 1;
    p2.position = Position(6, 5); p2.casier = Position(1, 3);
    p2.action = "CHERCHER"; p2.argument = "E";
    p2.perceptionN = "libre"; p2.perceptionS = "libre";
    p2.perceptionE = "libre"; p2.perceptionO = "armoire";
    p2.contenuCasier = "couverture";
    trace.pas.push_back(p2);

    Livraison ok;
    ok.demande = 1; ok.resident = "R1";
    ok.emotion = "tristesse"; ok.intensite = "moyenne";
    ok.casierChoisi = Position(1, 4);
    ok.repli = "aucun"; ok.objet = "couverture";
    ok.pasUtilises = 34; ok.succes = true;
    trace.livraisons.push_back(ok);

    Livraison ko;
    ko.demande = 2; ko.resident = "R2";
    ko.pasUtilises = 12; ko.succes = false;
    ko.motifEchec = "emotion indeterminee";
    trace.livraisons.push_back(ko);

    return trace;
}

static json relire() {
    std::ifstream f(FICHIER);
    assert(f.is_open());        
    return json::parse(f);
}

static void test_entete() {
    ecrireTrace(traceExemple(), FICHIER);
    json d = relire();
    assert(d["format"] == "robot-reconfort/trace");
    assert(d["version"] == 1);
    assert(d["carte"] == "appartement_01");
    assert(d["equipe"].size() == 2);
    std::cout << "OK trace (en-tête)" << std::endl;
}

static void test_pas() {
    ecrireTrace(traceExemple(), FICHIER);
    json d = relire();
    assert(d["pas"].size() == 2);
    assert(d["pas"][0]["position"] == json::array({6, 1}));
    assert(d["pas"][0]["perception"]["O"] == "mur");
    assert(d["pas"][0]["contenu_casier"].is_null());    // pas devant l'armoire
    assert(d["pas"][0]["commentaire"].is_null());
    assert(d["pas"][1]["contenu_casier"] == "couverture");
    std::cout << "OK trace (pas)" << std::endl;
}

static void test_livraisons_et_null() {
    ecrireTrace(traceExemple(), FICHIER);
    json d = relire();
    json ok = d["livraisons"][0];
    json ko = d["livraisons"][1];
    assert(ok["casier_choisi"] == json::array({1, 4}));
    assert(ok["motif_echec"].is_null());
    assert(ko["succes"] == false);
    assert(ko["motif_echec"] == "emotion indeterminee");
    assert(ko["emotion"].is_null());
    assert(ko["casier_choisi"].is_null());
    assert(ko["objet"].is_null());
    std::cout << "OK trace (livraisons, null)" << std::endl;
}

static void test_resume() {
    ecrireTrace(traceExemple(), FICHIER);
    json d = relire();
    assert(d["resume"]["demandes"] == 2);
    assert(d["resume"]["reussies"] == 1);
    assert(d["resume"]["echecs"] == 1);
    assert(d["resume"]["pas_total"] == 2);
    std::cout << "OK trace (résumé)" << std::endl;
}

int main() {
    test_entete();
    test_pas();
    test_livraisons_et_null();
    test_resume();
    std::cout << "Tous les tests trace passent." << std::endl;
    return 0;
}