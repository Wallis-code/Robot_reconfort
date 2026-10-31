#pragma once

#include <string>
#include <utility>

#include "JsonOpenerAbstract.hpp"

class Dictionnaire : public JsonOpenerAbstract {
private:
	json data;

public:
	Dictionnaire() = default;
	Dictionnaire(const char* fileName);

	std::pair<std::string, std::string> consulter(const std::string& phrase) const;
};
