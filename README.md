# Robot de réconfort — MOREAU Wallis & PRATVIEL Sébastien

Projet de semestre, master informatique. Un robot à perception locale
consulte un dictionnaire, fouille une armoire et rapporte un objet à un
résident.

Langage : C++ - Version : C++20 (g++)

## Lancer

```
make
./programme <carte.json> <scenario.json> <dossier_donnees> <trace_sortie.json>
```

Exemple :

```
./programme cartes/appartement_01.json cartes/scenario_01.json donnees sortie.json
```

Option visuelle : ajouter `--afficher` en 5e argument. Le terminal
affiche alors la carte mentale du robot à chaque tour, avec les cases
inconnues en `?`. La trace est écrite normalement.

Si un fichier d'entrée est absent, mal formé ou incohérent, le programme
affiche un message sur la sortie d'erreur, renvoie le code 1 et n'écrit
aucune trace.

Dépendances : aucune à installer. La bibliothèque nlohmann/json est
incluse dans le dépôt (`json.hpp`). Il faut seulement `g++` avec le
support du C++20, et `make`.

## Tester

```
make test
```

À lancer depuis la racine du dépôt. Les tests couvrent les 6 points suivants :
- planification et replanification (`test_planification`) 
- lecture du dictionnaire : un message par émotion, le premier mot
  l'emporte, aucun mot reconnu (`test_planification`) 
- bouclage 7 <-> 0  du sélecteur (`test_Armoire`) 
- les cas de repli : casier exact, intensité, colonne vide -> voisine,
  armoire vide (`test_fouille`) 
- un fichier cassé par catégorie (`test_validation`).
- le résident muré (`test_planification`).

S'y ajoute l'écriture de la trace (`test_trace`).

## Organisation du code

| Fichier | Rôle |
|---|---|
| `main.cpp` | point d'entrée : arguments, validation, boucle de simulation, écriture de la trace |
| `Robot.hpp` | boucle de décision (une action par tour), carte mentale, journalisation des pas et livraisons |
| `Parcours.hpp` | parcours en largeur sur la carte mentale, l'inconnu étant supposé libre |
| `Armoire.hpp/.cpp` | armoire 3 × 8 et sélecteur (`CHERCHER`, bouclage des colonnes) |
| `Candidat.hpp` | liste ordonnée des 15 casiers candidats |
| `Fouille.hpp` | fouille : itinéraire du sélecteur, casier choisi, étiquette de repli |
| `Dictionnaire.hpp/.cpp` | découpage du message en mots et recherche de l'émotion |
| `Trace.hpp` | structures `Pas`, `Livraison`, `Trace` et écriture du json |
| `VerificationCoherenceJSON.hpp` | validation des 4 fichiers d'entrée|
| `Grille.hpp`, `Case.hpp`, `Position.hpp` | grille, types de cases, positions et directions |
| `Appartement.cpp`, `Scenario.*`, `Demande.hpp` | lecture de la carte et du scénario |
| `Test.*`, `test_*.cpp` | tests (`make test`) |

## État

**Ce qui marche.** Les 5 cartes fournies produisent une trace complète et
cohérente : positions, perceptions, contenu des casiers et résumé. Le
robot ne connaît que ce qu'il perçoit, replanifie à chaque tour et
contourne les murs qu'il découvre. La règle de repli suit l'ordre de la
section 6.2 (distance, écart d'intensité, intensité la plus faible, sens
horaire), avec le bouclage. Les trois motifs d'échec sont gérés
(`resident inaccessible`, `armoire vide`, `emotion indeterminee`), et le
robot passe à la demande suivante sans planter. Un fichier cassé est
refusé avec le code 1, sans trace.

**Ce qui ne marche pas.**
- Le sujet ne donne pas de motif quand c'est l'armoire ou le dictionnaire
  qui est inaccessible. Nous utilisons `resident inaccessible`.
- Si le résident est finalement inaccessible alors que le robot tient déjà
  l'objet, la demande échoue et le robot repart les mains vides. Un
  simulateur qui rejoue la trace pourrait considérer l'objet comme
  toujours tenu au `PRENDRE` suivant, il y a un `PRENDRE` sans jamais avoir de `DONNER`.
- Sur une carte inconnue, l'hypothèse optimiste peut faire faire des
  allers-retours au robot avant qu'il découvre un mur, honnêtement on ne voit pas 
  comment améliorer ça.

## Vidéo

`demo.mp4`, également déclarée dans `rendu.json`.
