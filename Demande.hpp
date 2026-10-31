#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>

class Demande {
private:
	int numero;
	std::string IdRes;
	std::string message;

	static std::string enMinuscule(const std::string& texte) {
		std::string resultat = texte;
		for (char& caractere : resultat) {
			caractere = static_cast<char>(std::tolower(static_cast<unsigned char>(caractere)));
		}
		return resultat;
	}

	static std::string retireAccent(const std::string& message) // trouvé sur OpenClassrooms et adapté pour un const en argument
    {
        std::string accent("ÀÁÂÃÄÅàáâãäåÒÓÔÕÖØòóôõöøÈÉÊËèéêëÌÍÎÏìíîïÙÚÛÜùúûüÿÑñÇç");
        std::string sansAccent("AAAAAAaaaaaaOOOOOOooooooEEEEeeeeIIIIiiiiUUUUuuuuyNnCc");
		std::string resultat = message;     // Copie du message original dans resultat

		for (std::size_t i = 0; i + 1 < resultat.size(); ++i) {     // Parcours de la chaîne resultat
			for (std::size_t j = 0; j + 1 < accent.size(); j += 2) {   
				if (resultat[i] == accent[j] && resultat[i + 1] == accent[j + 1]) {
					resultat.replace(i, 2, 1, sansAccent[j / 2]);
					break;
				}
			}
		}

		return resultat;
    }

public:
	Demande(int numeroVal, std::string idResVal, std::string messageVal)
		: numero(numeroVal), IdRes(std::move(idResVal)), message(enMinuscule(retireAccent(messageVal))) {}

	static std::string retireAccent_public(const std::string& message){
		return retireAccent(message);
	}
	static std::string enMinuscule_public(const std::string& texte){
		return enMinuscule(texte);
	}

	int getNumero() const { return numero; }
	const std::string& getIdRes() const { return IdRes; }
	const std::string& getMessage() const { return message; }
};
