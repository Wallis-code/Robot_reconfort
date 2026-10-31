#include "Scenario.hpp"

#include <algorithm>
#include <iostream>

Scenario::Scenario(const char* fileName) {
	data = open(fileName, "robot-reconfort/scenario");
	if (data.is_null()) {
		std::cerr << "Format ou version du scenario incorrect" << std::endl;
		return;
	}

	for (const auto& demande : data["demandes"]) {
		demandes.emplace_back(
			demande["numero"].get<int>(),
			demande["resident"].get<std::string>(),
			demande["message"].get<std::string>()
		);
	}

    // Tri des demandes par numéro
    // Logique de https://www.geeksforgeeks.org/cpp/sort-c-stl/
    // Prérequis : data bien formé (numéros uniques, croissants à partir de 1)
	std::sort(demandes.begin(), demandes.end(), [](const Demande& premiere, const Demande& seconde) {
		return premiere.getNumero() < seconde.getNumero();
	});
}

const std::vector<Demande>& Scenario::getDemandes() const {
	return demandes;
}