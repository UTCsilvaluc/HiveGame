#include "Insecte.h"
Action* Insecte::actionPlacer(Hexagon targetCoord) {
    return new PlacementAction(this, targetCoord, this->owner);
}

Action* Insecte::actionDeplacer(Hexagon targetCoord) {
    return new DeplacementAction(this, this->getCoords() , targetCoord );
}

std::vector<Hexagon> Insecte::getVoisinsEnnemis(const std::vector<Hexagon>& voisins, const std::map<Hexagon, Insecte*>& plateau) const {
    std::vector<Hexagon> voisinsEnnemis;
    for (const auto& voisin : voisins) {
        auto it = plateau.find(voisin);
        if (it != plateau.end() && it->second != nullptr && it->second->getOwner() != this->getOwner()) {
            voisinsEnnemis.push_back(voisin);
        }
    }
    return voisinsEnnemis;
}
Insecte* trouverReine(Joueur* joueur, const std::map<Hexagon, Insecte*>& plateau) {
    for (auto it = plateau.begin(); it != plateau.end(); ++it) {
        Hexagon position = it->first;
        Insecte* insecte = it->second;
        if (insecte != nullptr && insecte->getOwner() == joueur && insecte->isQueen()) {
            return insecte;
        }
    }
    return nullptr;
}
std::vector<Hexagon> getVoisins(const Hexagon& coords) {
    std::vector<Hexagon> voisins;
    int q = coords.getQ();
    int r = coords.getR();
    if (r % 2 == 0) {  // Ligne paire
        voisins = {
            Hexagon(q - 1, r), //Ouest
            Hexagon(q + 1, r), //Est
            Hexagon(q, r - 1), //Nord-Est
            Hexagon(q, r + 1), //Sud-Est
            Hexagon(q - 1, r - 1), //Nord-Ouest
            Hexagon(q - 1, r + 1) //Sud-Ouest
        };
    } else {  // Ligne impaire
        voisins = {
            Hexagon(q - 1, r), //Ouest
            Hexagon(q + 1, r), //Est
            Hexagon(q + 1, r - 1), //Nord-Est
            Hexagon(q + 1, r + 1), //Sud-Est
            Hexagon(q, r - 1), //Nord-Ouest
            Hexagon(q, r + 1) //Sud-Ouest
        };
    }
    return voisins;
}


std::vector<Hexagon> casesAdjacentesVides(Hexagon coords, const std::map<Hexagon, Insecte*>& p) {
    std::vector<Hexagon> vides;
    std::vector<Hexagon> voisins = getVoisins(coords);

    for (const auto& voisin : voisins) {
        if (p.find(voisin) == p.end()) {
            vides.push_back(voisin);
        }
    }
    return vides;
}

std::vector<Hexagon> casesAdjacentesOccupees(Hexagon coords, const std::map<Hexagon, Insecte*>& p){
    std::vector<Hexagon> occupes;
    std::vector<Hexagon> voisins = getVoisins(coords);

    for (const auto& voisin : voisins) {
        if (p.find(voisin) != p.end()) {
            occupes.push_back(voisin);
        }
    }
    return occupes;
}

void getLongueurChaine(Hexagon coords, std::map<Hexagon, Insecte*> p, std::set<Hexagon> &chemin) {
    std::vector<Hexagon> voisinOccupee = casesAdjacentesOccupees(coords, p);
    for (Hexagon & voisin : voisinOccupee) {
        if (chemin.find(voisin) == chemin.end()) {
            chemin.insert(voisin);
            getLongueurChaine(voisin, p, chemin);
        }
    }
}

int Insecte::getTypeId() const {
    // Selon le nom ou un attribut type
    if (getNom() == "Reine") return 0;
    if (getNom() == "Fourmi") return 1;
    if (getNom() == "Scarabée") return 2;
    if (getNom() == "Sauterelle") return 3;
    if (getNom() == "Araignee") return 4;
    if (getNom() == "Coccinelle") return 5;
    if (getNom() == "Moustique") return 6;
    if (getNom() == "Termite") return 7;
    if (getNom() == "Papillon") return 8;
    // Ajoutez d’autres si besoin
    return -1; // Par défaut, erreur
}

bool getChaineBrisee(Hexagon coords, const std::map<Hexagon, Insecte*> p, std::set<Hexagon> &chemin) {
    std::map<Hexagon, Insecte*> p1 = p;
    if (p1.find(coords) != p1.end()) {
        p1.erase(coords);
    }
    if (p1.size() == 1) {
        return false;
    }
    Hexagon premierPion = p1.begin()->first;
    getLongueurChaine(premierPion, p1, chemin);
    return chemin.size() < p1.size();
}

bool getGlissementPossible(Hexagon coords, const  std::map<Hexagon, Insecte*> p, const  Hexagon destination){
    int t =0;
    std::vector<Hexagon> voisinOccupeeDepart = casesAdjacentesOccupees(coords, p);
    std::vector<Hexagon> voisinOccupeeArrivee = casesAdjacentesOccupees(destination, p);
    for(size_t j=0; j<voisinOccupeeDepart.size(); j++){
        for(size_t k=0; k<voisinOccupeeArrivee.size(); k++){
            if (voisinOccupeeDepart.at(j).getQ()==voisinOccupeeArrivee.at(k).getQ() && voisinOccupeeDepart.at(j).getR()==voisinOccupeeArrivee.at(k).getR()){
                t++;
            }
        }
    }
    if(t>=2){
        return false;
    }
    return true;
}

std::vector<Hexagon> deplacementsPossiblesReineAbeille(Hexagon coords, std::map<Hexagon, Insecte*> p){
    std::vector<Hexagon> deplacements;
    std::set<Hexagon> chemin;
    std::vector<Hexagon> voisinsVides = casesAdjacentesVides(coords, p);

    if (getChaineBrisee(coords, p, chemin)) {
        return deplacements;
    }

    std::map<Hexagon, Insecte*> p1 = p;
    p1.erase(coords);

    for (size_t i = 0; i < voisinsVides.size();) {
        if (casesAdjacentesOccupees(voisinsVides.at(i), p1).empty()) {
            voisinsVides.erase(voisinsVides.begin() + i);
        } else {
            ++i;
        }
    }

    for (size_t j = 0; j < voisinsVides.size();) {
        if (!getGlissementPossible(coords, p1, voisinsVides.at(j))) {
            voisinsVides.erase(voisinsVides.begin() + j);
        } else {
            ++j;
        }
    }
    return voisinsVides;
}

// Nouvelle méthode dans Insecte pour obtenir les placements possibles
std::vector<Hexagon> Insecte::getPlacementsPossibles(const std::map<Hexagon, Insecte*>& plateau) const {
    std::vector<Hexagon> placements;
    if (plateau.size() == 1) {
        Insecte* seulInsecte = plateau.begin()->second;
        if (seulInsecte == nullptr) {
            std::cerr << "Erreur : Aucun insecte trouvé sur le plateau." << std::endl;
            return {};  // Retourne un vecteur vide en cas d'erreur.
        }
        placements = getVoisins(seulInsecte->getCoords());
    } else {
        placements = this->placementsPossiblesDeBase(plateau);
    }
    return placements;
}

std::vector<Hexagon> ReineAbeille::deplacementsPossibles(std::map<Hexagon, Insecte*> p){
    return deplacementsPossiblesReineAbeille(getCoords(), p);
}

void deplacementsPossiblesFourmi(Hexagon coords, std::map<Hexagon, Insecte*> p, std::vector<Hexagon>& cheminInsecte, std::set<Hexagon>& deplacements) {
    std::set<Hexagon> cheminChaine;
    if(getChaineBrisee(coords, p, cheminChaine)){
        return;
    }
    std::vector<Hexagon> voisinsVides = casesAdjacentesVides(coords, p);
    std::map<Hexagon, Insecte*> p1 = p;
    p1.erase(coords);
    for (size_t i = 0; i < voisinsVides.size();) {
        if (casesAdjacentesOccupees(voisinsVides[i], p1).empty() || !getGlissementPossible(coords, p, voisinsVides[i])) {
            voisinsVides.erase(voisinsVides.begin() + i);
        } else {
            ++i;
        }
    }
    cheminInsecte.push_back(coords);
    for (Hexagon voisin : voisinsVides) {
        if (std::find(cheminInsecte.begin(), cheminInsecte.end(), voisin) == cheminInsecte.end()) {
            deplacements.insert(voisin);
            deplacementsPossiblesFourmi(voisin, p1, cheminInsecte, deplacements);
        }
    }
}

std::vector<Hexagon> Fourmi::deplacementsPossibles(std::map<Hexagon, Insecte*> p){
    std::set<Hexagon> deplacements;
    std::vector<Hexagon> cheminInsecte;
    deplacementsPossiblesFourmi(this->getCoords(), p, cheminInsecte, deplacements);
    std::vector<Hexagon> test = std::vector<Hexagon>(deplacements.begin(), deplacements.end());
    return std::vector<Hexagon>(deplacements.begin(), deplacements.end());
}

std::vector<Hexagon> deplacementsPossiblesScarabee(Hexagon coords, std::map<Hexagon, Insecte*> p){
    if (p[coords] == nullptr){
        return {};
    }
    if(p[coords]->getDessous()==nullptr){
        std::vector<Hexagon> deplacements;
        std::set<Hexagon> chemin;
        std::vector<Hexagon> voisinsOccupes = casesAdjacentesOccupees(coords, p);
        if(getChaineBrisee(coords, p, chemin)){
            return deplacements;
        }
        std::vector<Hexagon> voisinsVides = casesAdjacentesVides(coords, p);
        std::map<Hexagon, Insecte*> p1 = p;
        p1.erase(coords);
        for (size_t i = 0; i < voisinsVides.size();) {
            if (casesAdjacentesOccupees(voisinsVides[i], p1).empty() || !getGlissementPossible(coords, p, voisinsVides[i])) {
                voisinsVides.erase(voisinsVides.begin() + i);
            } else {
                ++i;
            }
        }
        deplacements = voisinsVides;
        deplacements.insert(deplacements.end(), voisinsOccupes.begin(), voisinsOccupes.end());
        return deplacements;
    }
    else{
        Insecte* s = p[coords]->getDessous();
        p.insert(std::make_pair(coords, s));
        std::vector<Hexagon> voisins = getVoisins(coords);
        return voisins;
    }
}

std::vector<Hexagon> Scarabee::deplacementsPossibles(std::map<Hexagon, Insecte*> p){
    return deplacementsPossiblesScarabee(getCoords(), p);
}

void deplacementsPossiblesCoccinelle(Hexagon coords, std::map<Hexagon, Insecte*> p, int i, std::vector<Hexagon>& cheminFinal, std::set<Hexagon>& visited) {
    if (visited.find(coords) != visited.end()) {
        return;
    }
    visited.insert(coords);

    if (i > 0) {
        std::vector<Hexagon> voisinsOccupes = casesAdjacentesOccupees(coords, p);
        for (Hexagon voisin : voisinsOccupes) {
            deplacementsPossiblesCoccinelle(voisin, p, i - 1, cheminFinal, visited);
        }
    } else {
        std::vector<Hexagon> voisinsVides = casesAdjacentesVides(coords, p);
        for (Hexagon voisinVide : voisinsVides) {
            if (visited.find(voisinVide) == visited.end()) {
                cheminFinal.push_back(voisinVide);
                visited.insert(voisinVide);
            }
        }
    }
}

std::vector<Hexagon> Coccinelle::deplacementsPossibles(std::map<Hexagon, Insecte*> p) {
    std::set<Hexagon> chemin;
    if (getChaineBrisee(this->getCoords(), p, chemin)) {
        return {};
    }
    int i = 2;
    std::vector<Hexagon> cheminFinal;
    std::set<Hexagon> visited;
    deplacementsPossiblesCoccinelle(this->getCoords(), p, i, cheminFinal, visited);

    return cheminFinal;
}

void deplacementsPossiblesSauterelle(Hexagon coords, std::map<Hexagon, Insecte*> p, std::vector<Hexagon>& cheminFinal) {
    std::vector<Hexagon> voisins = casesAdjacentesOccupees(coords, p);

    for (Hexagon voisin : voisins) {
        Hexagon current = voisin;
        bool caseVide = false;
        int dQ = voisin.getQ() - coords.getQ();
        int dR = voisin.getR() - coords.getR();

        if (coords.getR() % 2 != 0) {
            if (dR == -1) dQ += 1;
            if (dR == 1) dQ += 1;
        } else {
            if (dR == -1) dQ += 1;
            if (dR == 1) dQ += 1;
        }

        while (!caseVide) {
            if (p.find(current) == p.end()) {
                cheminFinal.push_back(current);
                caseVide = true;
            } else {
                current = Hexagon(current.getQ() + dQ, current.getR() + dR);
            }
        }
    }
}

std::vector<Hexagon> Sauterelle::deplacementsPossibles(std::map<Hexagon, Insecte*> p) {
    std::set<Hexagon> chemin;
    if (getChaineBrisee(this->getCoords(), p, chemin)) {
        return {};
    }
    std::vector<Hexagon> cheminFinal;
    deplacementsPossiblesSauterelle(this->getCoords(), p, cheminFinal);
    std::vector<Hexagon> voisinsVides = casesAdjacentesVides(this->getCoords(), p);
    std::map<Hexagon, Insecte*> p1 = p;
    p1.erase(this->getCoords());
    for (size_t i = 0; i < voisinsVides.size();) {
        if (casesAdjacentesOccupees(voisinsVides[i], p1).empty() || !getGlissementPossible(this->getCoords(), p, voisinsVides[i])) {
            voisinsVides.erase(voisinsVides.begin() + i);
        } else {
            ++i;
        }
    }
    cheminFinal.insert(cheminFinal.end(), voisinsVides.begin(), voisinsVides.end());
    return std::vector<Hexagon>(cheminFinal.begin(), cheminFinal.end());
}

void deplacementsPossiblesAraignee(Hexagon coords, std::map<Hexagon, Insecte*> p, int i, std::vector<Hexagon>& cheminInsecte, std::set<Hexagon>& deplacementsFinaux) {
    std::set<Hexagon> cheminChaine;
    if(getChaineBrisee(coords, p, cheminChaine)){
        return;
    }
    std::vector<Hexagon> voisinsVides = casesAdjacentesVides(coords, p);
    std::map<Hexagon, Insecte*> p1 = p;
    p1.erase(coords);
    for (size_t j = 0; j < voisinsVides.size();) {
        if (casesAdjacentesOccupees(voisinsVides[j], p1).empty() || !getGlissementPossible(coords, p, voisinsVides[j])) {
            voisinsVides.erase(voisinsVides.begin() + j);
        } else {
            ++j;
        }
    }
    cheminInsecte.push_back(coords);
    if(i>0){
        for (Hexagon voisin : voisinsVides) {
            if (std::find(cheminInsecte.begin(), cheminInsecte.end(), voisin) == cheminInsecte.end()) {
                deplacementsPossiblesAraignee(voisin, p1, i-1, cheminInsecte, deplacementsFinaux);
            }
        }
    }
    else if (cheminInsecte.size() == 4) {
        deplacementsFinaux.insert(coords);
    }
    cheminInsecte.pop_back();
}

std::vector<Hexagon> Araignee::deplacementsPossibles(std::map<Hexagon, Insecte*> p){
    int i=3;
    std::set<Hexagon> deplacementsFinaux;
    std::vector<Hexagon> cheminInsecte;
    deplacementsPossiblesAraignee(this->getCoords(), p, i, cheminInsecte, deplacementsFinaux);
    return std::vector<Hexagon>(deplacementsFinaux.begin(), deplacementsFinaux.end());
}

std::vector<Hexagon> Moustique::deplacementsPossibles(std::map<Hexagon, Insecte*> p) {
    Hexagon coords = this->getCoords();
    std::vector<Hexagon> voisins = casesAdjacentesOccupees(coords, p);
    std::set<Hexagon> deplacementsUnique;

    for (const Hexagon& voisin : voisins) {
        auto it = p.find(voisin);
        if (it != p.end()) {
            Insecte* insecte = it->second;
            if (insecte != nullptr) {
                // Vérifier si l'insecte n'est pas un Moustique
                if (dynamic_cast<Moustique*>(insecte) == nullptr) {
                    // Ajouter les déplacements possibles de cet insecte
                    std::vector<Hexagon> deplacementsInsecte = insecte->deplacementsPossibles(p);
                    deplacementsUnique.insert(deplacementsInsecte.begin(), deplacementsInsecte.end());
                }
            }
        }
    }

    return std::vector<Hexagon>(deplacementsUnique.begin(), deplacementsUnique.end());
}

bool ReineAbeille::estEntouree(const std::map<Hexagon, Insecte*>& p) const {
    std::vector<Hexagon> voisins = getVoisins(getCoords());
    for (const Hexagon& voisin : voisins) {
        if (p.find(voisin) == p.end()) {
            return false; // Si une case voisine est vide, la reine n'est pas entourée
        }
    }
    return true;
}

std::string Insecte::toJson() const {
    std::stringstream jsonData;

    // Identifiant unique basé sur l'adresse mémoire (unique ID)
    jsonData << static_cast<const void*>(this) << ";";

    // Nom de l'insecte
    jsonData << nom << ";";

    // Coordonnées de l'insecte (en supposant que la méthode toJson de Coord retourne une chaîne)
    jsonData << coords.toJson() << ";";

    // Propriétaire, ou "null" si l'insecte n'a pas de propriétaire
    jsonData << (owner ? owner->getName() : "null") << ";";

    // Dessus, ou "null" si l'insecte n'a pas de dessus
    jsonData << (dessus ? std::to_string(reinterpret_cast<std::uintptr_t>(dessus)) : "null") << ";";

    // Dessous, ou "null" si l'insecte n'a pas de dessous
    jsonData << (dessous ? std::to_string(reinterpret_cast<std::uintptr_t>(dessous)) : "null");

    return jsonData.str();
}


std::vector<Hexagon> Insecte::placementsPossiblesDeBase(const std::map<Hexagon, Insecte*>& plateau) const {
    std::vector<Hexagon> validPositions;
    std::vector<Hexagon> enemyNeighbors;

    // Étape 1 : Trouver les cases vides adjacentes aux insectes alliés
    std::vector<Hexagon> potentialPositions;
    for (const auto& pair : plateau) {
        Insecte* insecte = pair.second;
        if (insecte && insecte->getOwner() == this->owner) {
            std::vector<Hexagon> videsAdjacents = casesAdjacentesVides(insecte->getCoords(), plateau);
            potentialPositions.insert(potentialPositions.end(), videsAdjacents.begin(), videsAdjacents.end());
        } else if (insecte) {
            std::vector<Hexagon> voisins = insecte->getCoords().getVoisins();
            enemyNeighbors.insert(enemyNeighbors.end(), voisins.begin(), voisins.end());
        }
    }

    // Étape 2 : Filtrer les cases vides sans ennemis adjacents
    for (const auto& caseVide : potentialPositions) {
        if (std::find(enemyNeighbors.begin(), enemyNeighbors.end(), caseVide) == enemyNeighbors.end()) {
            validPositions.push_back(caseVide);
        }
    }

    return validPositions;
}


std::vector<Hexagon> Termite::deplacementsPossibles(std::map<Hexagon, Insecte*> p){
    return deplacementsPossiblesScarabee(getCoords(), p);
}
Action* Termite::actionDeplacer(Hexagon targetCoord) {
    // La Termite exécute une action spécifique : MangerPionAction
    return new MangerPionAction(this, this->getCoords(), targetCoord);
}
int calculerNombreDeVoisins(const std::map<Hexagon, Insecte*>& plateau, const Hexagon& position) {
    std::vector<Hexagon> voisins = getVoisins(position);
    int count = 0;
    for (const auto& v : voisins) {
        if (plateau.find(v) != plateau.end() && plateau.at(v) != nullptr) {
            count++;
        }
    }
    return count;
}




