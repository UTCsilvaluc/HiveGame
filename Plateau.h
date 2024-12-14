#ifndef PLATEAU_H
#define PLATEAU_H

#include "Insecte.h"
#include "Hexagon.h"
#include <map>
#include <vector>
#include <limits>
#include <iostream>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
class Insecte;
class Joueur;
class Plateau {
private:
    std::map<Hexagon, Insecte*> plateauMap;
    int nombreRetoursArriere;
    int minR, maxR, minQ, maxQ;

public:
    Plateau(std::map<Hexagon, Insecte*> pMap) : nombreRetoursArriere(3), minR(0), maxR(0), minQ(0), maxQ(0) , plateauMap(pMap) {} // Initialisation par d�faut
    Plateau() : nombreRetoursArriere(3), minR(0), maxR(0), minQ(0), maxQ(0) {} // Initialisation par d�faut
    void afficherPlateauAvecPossibilites(const std::vector<Hexagon>& emplacementsPossibles, Joueur* j1, Joueur* j2, Joueur* current);
    void afficherPossibilitesDeplacements(const Insecte* insecte, const std::vector<Hexagon> deplacementsPossibles) const;
    void afficherPossibilitesPlacements(const Insecte* insecte, const std::vector<Hexagon> placementsPossibles) const;
    std::vector<Hexagon> getPlacementsPossibles(Insecte* insecte);
    void mettreAJourLimites() {
        // Initialiser les limites � des valeurs extr�mes
        minR = std::numeric_limits<int>::max();
        maxR = std::numeric_limits<int>::min();
        minQ = std::numeric_limits<int>::max();
        maxQ = std::numeric_limits<int>::min();
        for (const auto& pair : plateauMap) {
            const auto& coords = pair.first;
            const auto& insecte = pair.second;
            if (coords.getR() < minR) minR = coords.getR();
            if (coords.getR() > maxR) maxR = coords.getR();
            if (coords.getQ() < minQ) minQ = coords.getQ();
            if (coords.getQ() > maxQ) maxQ = coords.getQ();
        }
    }

    void ajouterInsecte(Insecte* insecte, Hexagon position);

    void deplacerInsecte(Insecte* insecte, const Hexagon& nouvellePosition);

    void superposerInsecte(Insecte* insecteExistant, Insecte* newInsecte);

    void afficherPlateau(Joueur *p1, Joueur *p2) const;
    void setPlateauMap(std::map<Hexagon, Insecte*> pMap){plateauMap = pMap;}

    int getMinR() const { return minR; }
    int getMaxR() const { return maxR; }
    int getMinQ() const { return minQ; }
    int getMaxQ() const { return maxQ; }

    Insecte* getInsecteAt(const Hexagon& position) const {
        if (plateauMap.count(position)) {
            return plateauMap.at(position);  // Utilisation de .at() pour éviter une insertion involontaire
        }
        return nullptr;
    }

    std::vector<Hexagon> getVoisinsInsectePlateau(Insecte *insecte);


    Insecte* getReineAbeille(Joueur* joueur) const ;
    bool estEntouree(Insecte *insecte);


    bool plateauEstVide();

    bool playerCanMoveInsecte(Joueur* joueur) ;



    Insecte* getSeulInsecteSurPlateau() const;
    void supprimerInsecte(const Hexagon& position);
    const std::map<Hexagon, Insecte*>& getPlateauMap() const ;
    void supprimerInsectePlateauCoords(const Hexagon& position);
    std::string toJson() const ;


};

#endif // PLATEAU_H
