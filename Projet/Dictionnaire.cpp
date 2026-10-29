#include "Dictionnaire.hpp"

#include <iostream>
#include <sstream>

Dictionnaire::Dictionnaire(const char* fileName) {
	data = open(fileName, "robot-reconfort/dictionnaire");
	if (data.is_null()) {
		std::cerr << "Format ou version du dictionnaire incorrect" << std::endl;
	}
}


// Découpe une phrase
// tout ce qui n'est pas une lettre sépare deux mots (section 6.1)
// "j'entends du bruit." -> j, entends, du, bruit
static std::vector<std::string> decouperEnMots(const std::string& phrase) {
	std::vector<std::string> mots;
	std::string courant;
	for (char ch : phrase) {
		if (ch >= 'a' && ch <= 'z') {
			courant += ch;              // une lettre : on continue le mot
		} else if (!courant.empty()) {
			mots.push_back(courant);    // autre chose : le mot est fini
			courant.clear();
		}
	}
	if (!courant.empty()) mots.push_back(courant);   // dernier mot de la phrase
	return mots;
}

// In : std::string déjà formaté (minuscule, sans accent) (prérequis)
// Out : {emotion, intensite} ou {"", ""} si le mot n'est pas trouvé
// Logique de https://www.geeksforgeeks.org/cpp/split-a-sentence-into-words-in-cpp/
//j'ai modifié car istringstream faisait n'importe quoi avec les ' et du coup les demandes marchait pas
std::pair<std::string, std::string> Dictionnaire::consulter(const std::string& phrase) const {
	for (const std::string& currMot : decouperEnMots(phrase)) {   // Parcours de chaque mot de la phrase
		for (const auto& entree : data["entrees"]) {    // parcours de chaque entrée du dictionnaire
			for (const auto& forme : entree["formes"]) {    // parcours de chaque forme du mot
				if (currMot == forme.get<std::string>()) {      // comparaison avec le current mot
					return {                                    // trouvé : return de l'émotion et de l'intensité
						entree["emotion"].get<std::string>(),
						entree["intensite"].get<std::string>()
					};
				}
			}
		}
	}

	return {"", ""}; // la demande échoue avec le motif emotion indeterminee (à implémenter)
}