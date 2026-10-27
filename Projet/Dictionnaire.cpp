#include "Dictionnaire.hpp"

#include <iostream>
#include <sstream>

Dictionnaire::Dictionnaire(const char* fileName) {
	data = open(fileName, "robot-reconfort/dictionnaire");
	if (data.is_null()) {
		std::cerr << "Format ou version du dictionnaire incorrect" << std::endl;
	}
}

// In : std::string déjà formaté (minuscule, sans accent) (prérequis)
// Out : {emotion, intensite} ou {"", ""} si le mot n'est pas trouvé
// Logique de https://www.geeksforgeeks.org/cpp/split-a-sentence-into-words-in-cpp/
std::pair<std::string, std::string> Dictionnaire::consulter(const std::string& phrase) const {
	std::istringstream mots(phrase);
	std::string currMot;

	while (mots >> currMot) {   // Parcours de chaque mot de la phrase
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