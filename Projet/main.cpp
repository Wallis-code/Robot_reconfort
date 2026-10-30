#include "Appartement.cpp"
#include "Armoire.hpp"
#include "Candidat.hpp"
#include "Demande.hpp"
#include "Dictionnaire.hpp"
#include "Scenario.hpp"
#include "Robot.hpp"
#include "Trace.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

// Lit un fichier json entier (seulement pour récupérer quelques champs comme "nom")
static json lireJson(const std::string& chemin) {
    std::ifstream f(chemin);
    if (!f) throw std::runtime_error("Impossible d'ouvrir " + chemin);
    return json::parse(f);
}

// Contrat d'appel (section 2 du sujet) :
//   ./programme <carte> <scenario> <dossier de données> <trace à écrire>
int main(int argc, char** argv)
{
    if (argc != 5) {
        std::cerr << "Usage : " << argv[0]
                  << " <carte.json> <scenario.json> <dossier_donnees> <trace.json>" << std::endl;
        return 1;
    }

    const std::string cheminCarte    = argv[1];
    const std::string cheminScenario = argv[2];
    const std::string dossierDonnees = argv[3];
    const std::string cheminTrace    = argv[4];

    try {
        // CHARGEMENT DES FICHIERS
        // le nom de l'armoire est donné par le scénario
        json infosScenario = lireJson(cheminScenario);
        json infosCarte    = lireJson(cheminCarte);
        //on recupère que les données d'un truc précis dans le json
        const std::string cheminArmoire = dossierDonnees + "/" + infosScenario.at("armoire").get<std::string>() + ".json";
        const std::string cheminDico    = dossierDonnees + "/dictionnaire.json";

        //c_str() passe d'un string à un char*
        Dictionnaire dictionnaire(cheminDico.c_str());
        Scenario scenario(cheminScenario.c_str());
        const std::vector<Demande>& demandes = scenario.getDemandes();
        Armoire armoire(cheminArmoire.c_str());
        Appartement appart(cheminCarte.c_str());
        Grille grille = appart.getGrille();

        //----------------------- ROBOT ----------
        Robot robot(grille, demandes, dictionnaire, armoire);

        // AU CAS OU
        const int MAX_TOURS = 10000;
        int tours = 0;
        while (robot.getNumeroDemandeCourante() <= (int)demandes.size() && tours < MAX_TOURS) {
            robot.choixAction();
            tours++;
        }
        if (tours == MAX_TOURS) {
            //normalement là y'a un pb, possiblement modifié MAX_TOURS
            std::cerr << "Attention : nombre maximum de tours atteint" << std::endl;
        }

        // -------------------TRACE--------------------------------
        Trace trace = robot.getTrace();
        trace.carte    = infosCarte.at("nom").get<std::string>();
        trace.scenario = infosScenario.at("nom").get<std::string>();
        trace.equipe   = {"MOREAU Wallis", "PRATVIEL Sebastien"};
        ecrireTrace(trace, cheminTrace);
    }
    catch (const std::exception& e) {
        // erreur de données
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}