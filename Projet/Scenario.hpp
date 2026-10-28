#pragma once

#include <vector>

#include "Demande.hpp"
#include "JsonOpenerAbstract.hpp"

class Scenario : public JsonOpenerAbstract {
private:
	json data;
	std::vector<Demande> demandes;

public:
	Scenario(const char* fileName);

	const std::vector<Demande>& getDemandes() const;
};
