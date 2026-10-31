#pragma once

#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "json.hpp"

using json = nlohmann::json;

class VerificationCoherenceJSON {
private:
	static bool chaineNonVide(const json& objet, const char* champ) {
		return objet.contains(champ) && objet[champ].is_string() && !objet[champ].get<std::string>().empty();
	}

	static bool positionValide(const json& position, int hauteur, int largeur) {
		return position.is_array() && position.size() == 2 &&
			   position[0].is_number_integer() && position[1].is_number_integer() &&
			   position[0].get<int>() >= 0 && position[0].get<int>() < hauteur &&
			   position[1].get<int>() >= 0 && position[1].get<int>() < largeur;
	}

	static bool formatValide(const json& document, const char* format) {
		return document.is_object() && document.contains("version") &&
			   document["version"].is_number_integer() && document["version"] == 1 &&
			   document.contains("format") && document["format"] == format;
	}

	static bool verifierCarte(const json& carte) {
		if (!formatValide(carte, "robot-reconfort/carte") || !chaineNonVide(carte, "nom")) {
			return false;
		}

		if (!carte.contains("dimensions") || !carte["dimensions"].is_object()) return false;
		const auto& dimensions = carte["dimensions"];
		if (!dimensions.contains("hauteur") || !dimensions.contains("largeur") ||
			!dimensions["hauteur"].is_number_integer() || !dimensions["largeur"].is_number_integer()) {
			return false;
		}

		const int hauteur = dimensions["hauteur"].get<int>();
		const int largeur = dimensions["largeur"].get<int>();
		if (hauteur <= 0 || largeur <= 0 || !carte.contains("grille") || !carte["grille"].is_array() ||
			static_cast<int>(carte["grille"].size()) != hauteur) {
			return false;
		}

		int nombreDepart = 0;
		int nombreArmoires = 0;
		int nombreDictionnaires = 0;
		int nombreResidents = 0;
		for (const auto& ligne : carte["grille"]) {
			if (!ligne.is_string() || static_cast<int>(ligne.get<std::string>().size()) != largeur) return false;
			for (char symbole : ligne.get<std::string>()) {
				if (std::string("#.RADP").find(symbole) == std::string::npos) return false;
				if (symbole == 'R') ++nombreDepart;
				if (symbole == 'A') ++nombreArmoires;
				if (symbole == 'D') ++nombreDictionnaires;
				if (symbole == 'P') ++nombreResidents;
			}
		}

		if (nombreDepart != 1 || nombreArmoires != 1 || nombreDictionnaires != 1) return false;

		if (!carte.contains("depart_robot") || !positionValide(carte["depart_robot"], hauteur, largeur) ||
			!carte.contains("armoire") || !carte["armoire"].is_object() ||
			!carte["armoire"].contains("position") ||
			!positionValide(carte["armoire"]["position"], hauteur, largeur) ||
			!carte.contains("dictionnaire") || !carte["dictionnaire"].is_object() ||
			!carte["dictionnaire"].contains("position") ||
			!positionValide(carte["dictionnaire"]["position"], hauteur, largeur)) {
			return false;
		}

		if (!carte.contains("residents") || !carte["residents"].is_array()) return false;
		if (nombreResidents != static_cast<int>(carte["residents"].size())) return false;
		std::set<std::string> ids;
		for (const auto& resident : carte["residents"]) {
			if (!resident.is_object() || !chaineNonVide(resident, "id") ||
				!chaineNonVide(resident, "nom") || !resident.contains("position") ||
				!positionValide(resident["position"], hauteur, largeur) ||
				carte["grille"][resident["position"][0].get<int>()].get<std::string>()[
					resident["position"][1].get<int>()] != 'P' ||
				!ids.insert(resident["id"].get<std::string>()).second) {
				return false;
			}
		}

		const auto symbolePosition = [&carte](const json& position) {
			return carte["grille"][position[0].get<int>()].get<std::string>()[position[1].get<int>()];
		};
		if (symbolePosition(carte["depart_robot"]) != 'R' ||
			symbolePosition(carte["armoire"]["position"]) != 'A' ||
			symbolePosition(carte["dictionnaire"]["position"]) != 'D') {
			return false;
		}

		return true;
	}

	static bool verifierArmoire(const json& armoire) {
		if (!formatValide(armoire, "robot-reconfort/armoire") || !chaineNonVide(armoire, "nom") ||
			!armoire.contains("casier_depart") || !armoire["casier_depart"].is_array() ||
			armoire["casier_depart"].size() != 2 ||
			!armoire["casier_depart"][0].is_number_integer() ||
			!armoire["casier_depart"][1].is_number_integer() ||
			armoire["casier_depart"][0].get<int>() < 0 || armoire["casier_depart"][0].get<int>() >= 3 ||
			armoire["casier_depart"][1].get<int>() < 0 || armoire["casier_depart"][1].get<int>() >= 8 ||
			!armoire.contains("casiers") ||
			!armoire["casiers"].is_array()) {
			return false;
		}

		std::set<std::pair<int, int>> positions;
		for (const auto& casier : armoire["casiers"]) {
			if (!casier.is_object() || !casier.contains("ligne") || !casier.contains("colonne") ||
				!casier["ligne"].is_number_integer() || !casier["colonne"].is_number_integer() ||
				casier["ligne"].get<int>() < 0 || casier["ligne"].get<int>() >= 3 ||
				casier["colonne"].get<int>() < 0 || casier["colonne"].get<int>() >= 8 ||
				!chaineNonVide(casier, "emotion") || !chaineNonVide(casier, "intensite") ||
				(!casier["objet"].is_null() && !casier["objet"].is_string()) ||
				!positions.insert({casier["ligne"].get<int>(), casier["colonne"].get<int>()}).second) {
				return false;
			}
		}

		return positions.size() == 24;
	}

	static bool verifierDictionnaire(const json& dictionnaire) {
		if (!formatValide(dictionnaire, "robot-reconfort/dictionnaire") ||
			!chaineNonVide(dictionnaire, "nom") ||
			!dictionnaire.contains("emotions") || !dictionnaire["emotions"].is_array() ||
			!dictionnaire.contains("intensites") || !dictionnaire["intensites"].is_array() ||
			!dictionnaire.contains("entrees") || !dictionnaire["entrees"].is_array()) {
			return false;
		}

		for (const auto& emotion : dictionnaire["emotions"]) {
			if (!emotion.is_string() || emotion.get<std::string>().empty()) return false;
		}
		for (const auto& intensite : dictionnaire["intensites"]) {
			if (!intensite.is_string() || intensite.get<std::string>().empty()) return false;
		}

		for (const auto& entree : dictionnaire["entrees"]) {
			if (!entree.is_object() || !entree.contains("formes") || !entree["formes"].is_array() ||
				entree["formes"].empty() || !chaineNonVide(entree, "emotion") ||
				!chaineNonVide(entree, "intensite")) {
				return false;
			}
			for (const auto& forme : entree["formes"]) {
				if (!forme.is_string() || forme.get<std::string>().empty()) return false;
			}
		}

		return true;
	}

	static bool verifierScenario(const json& scenario, const json& carte, const json& armoire) {
		if (!formatValide(scenario, "robot-reconfort/scenario") || !chaineNonVide(scenario, "nom") ||
			!chaineNonVide(scenario, "carte") || !chaineNonVide(scenario, "armoire") ||
			scenario["carte"] != carte["nom"] || scenario["armoire"] != armoire["nom"] ||
			!scenario.contains("demandes") || !scenario["demandes"].is_array()) {
			return false;
		}

		std::set<std::string> residents;
		for (const auto& resident : carte["residents"]) residents.insert(resident["id"].get<std::string>());

		std::set<int> numeros;
		for (const auto& demande : scenario["demandes"]) {
			if (!demande.is_object() || !demande.contains("numero") ||
				!demande["numero"].is_number_integer() || demande["numero"].get<int>() <= 0 ||
				!chaineNonVide(demande, "resident") || !chaineNonVide(demande, "message") ||
				!residents.contains(demande["resident"].get<std::string>()) ||
				!numeros.insert(demande["numero"].get<int>()).second) {
				return false;
			}
		}

		return true;
	}

public:
	static bool verifier(const json& carte, const json& scenario, const json& armoire) {
		return verifierCarte(carte) && verifierArmoire(armoire) && verifierScenario(scenario, carte, armoire);
	}

	static bool verifier(const json& carte, const json& scenario,
		const json& armoire, const json& dictionnaire) {
		return verifierCarte(carte) && verifierArmoire(armoire) &&
			verifierDictionnaire(dictionnaire) && verifierScenario(scenario, carte, armoire);
	}

	static bool verifierFichiers(const char* fichierCarte, const char* fichierScenario, const char* fichierArmoire) {
		try {
			std::ifstream carteStream(fichierCarte);
			std::ifstream scenarioStream(fichierScenario);
			std::ifstream armoireStream(fichierArmoire);
			if (!carteStream || !scenarioStream || !armoireStream) return false;

			return verifier(json::parse(carteStream), json::parse(scenarioStream), json::parse(armoireStream));
		} catch (const json::exception&) {
			return false;
		}
	}
};
