#include "Joueur.h"
std::vector<Insecte*> deckDeBase(Joueur *joueur){
    std::vector<Insecte*> newDeck;
    for (int i = 0; i < 2; ++i) { newDeck.push_back(new Araignee(Hexagon(0, 0) , joueur)); }  // Araign�e x2
    for (int i = 0; i < 2; ++i) { newDeck.push_back(new Scarabee(Hexagon(0, 0) , joueur)); }  // Scarab�e x2
    for (int i = 0; i < 3; ++i) { newDeck.push_back(new Fourmi(Hexagon(0, 0) , joueur)); }    // Fourmi x3
    for (int i = 0; i < 3; ++i) { newDeck.push_back(new Sauterelle(Hexagon(0, 0) , joueur)); } // Sauterelle x3
    newDeck.push_back(new Coccinelle(Hexagon(0, 0) , joueur));  // Coccinelle x1
    newDeck.push_back(new Moustique(Hexagon(0, 0) , joueur));   // Moustique x1
    newDeck.push_back(new ReineAbeille(Hexagon(0, 0) , joueur));  // Reine Abeille x1
    newDeck.push_back(new Termite(Hexagon(0, 0) , joueur));  // Reine Abeille x1
    return newDeck;
}
void Joueur::afficherDeck() const {
    for (auto it = deck.begin(); it != deck.end(); ++it) {
        std::cout <<(it - deck.begin()) + 1
                  << ": " << (*it)->getNom() << std::endl;
    }
}

int Joueur::findInsectIndexInDeck(const std::vector<Insecte*>& deck, Insecte* insecte) {
    for (size_t i = 0; i < deck.size(); ++i) {
        if (deck[i] == insecte) {
            return static_cast<int>(i);
        }
    }
    return -1; // Retourne -1 si l'insecte n'est pas trouvé
}


Insecte* Joueur::getQueenOnPlateau(const std::map<Hexagon, Insecte*>& plateau) const {
    for (const auto& [position, insecte] : plateau) {
        if (insecte && insecte->getOwner() == this && insecte->isQueen()) {
            return insecte;
        }
    }
    return nullptr; // Retourne nullptr si la Reine n'est pas trouvée
}


Insecte* Joueur::getQueen() const {
    for (auto it = deck.begin(); it != deck.end(); ++it) {
        if ((*it)->getNom() == "Reine") {
            return *it; // Retourne un pointeur vers la Reine concr�te
        }
    }
    return nullptr;
}
int Joueur::getQueenIndex() const {
    for (size_t i = 0; i < deck.size(); ++i) {
        if (deck[i]->getNom() == "Reine") {
            return i;  // Retourne l'index si la reine est trouv�e
        }
    }
    return -1;
}
bool Joueur::hasQueen() const {
    for (const auto& insecte : deck) {
        if (insecte->getNom() == "Reine") {
            return true;
        }
    }
    return false;
}

int getInput(const std::string& prompt, int minValue, int maxValue, unsigned int tour) {
    std::cout<<tour<< "est le tour en cours"<<std::endl;
    int choice;
    if (tour == 0){
        minValue = std::numeric_limits<int>::min();
        maxValue = std::numeric_limits<int>::max();
    }
    while (true) {
        std::cout << prompt;
        std::cin >> choice;
        if (std::cin.fail() || choice < minValue || choice > maxValue) {
            std::cin.clear(); // R�initialiser l'�tat d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer les caract�res restants
            std::cout << "S'il vous pla�t, entrez un nombre valide entre " << minValue << " et " << maxValue << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer les caract�res restants
            break; // Sortir de la boucle si l'entr�e est valide
        }
    }
    std::cin.clear();
    return choice;
}

int getInput(const std::string& prompt, int minValue, int maxValue) {
    int choice;
    while (true) {
        std::cout << prompt;
        std::cin >> choice;

        if (std::cin.fail() || choice < minValue || choice > maxValue) {
            std::cin.clear(); // R�initialiser l'�tat d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer les caract�res restants
            std::cout << "S'il vous pla�t, entrez un nombre valide entre " << minValue << " et " << maxValue << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer les caract�res restants
            break; // Sortir de la boucle si l'entr�e est valide
        }
    }
    std::cin.clear();
    return choice;
}

#include <sstream>
std::string Joueur::toJson() const {
    std::stringstream jsonData;
    jsonData << "{\n";
    jsonData << "  \"nom\": \"" << nom << "\",\n";

    jsonData << "  \"deck\": [";
    for (size_t i = 0; i < deck.size(); ++i) {
        if (i != 0) {
            jsonData << ", ";  // S�parateur entre les insectes
        }
        jsonData << deck[i]->toJson();  // Appel de toJson pour chaque Insecte
    }
    jsonData << "]\n";

    jsonData << "}";
    return jsonData.str();
}

void JoueurIANiveau2::protegerReine(const std::map<Hexagon, Insecte*>& plateau) {
    // Récupérer la Reine du joueur sur le plateau
    Insecte* reine = getQueenOnPlateau(plateau);
    if (!reine) {
        candidats.clear(); // Pas de Reine, donc pas de candidats
        return;
    }

    // Effacer les candidats précédents
    candidats.clear();

    // Récupérer les voisins de la Reine et déterminer les ennemis
    std::vector<Hexagon> voisinsReine = getVoisins(reine->getCoords());
    std::vector<Hexagon> ennemisVoisins = reine->getVoisinsEnnemis(voisinsReine, plateau);

    // Ajouter les déplacements possibles de la Reine
    verifierDeplacementsReine(reine, ennemisVoisins, plateau);

    // Ajouter les déplacements possibles des alliés
    verifierDeplacementsAllies(reine, voisinsReine, plateau);

    if (!candidats.empty()) {
        actionChoisie = DEPLACER;
    }
}



void JoueurIANiveau2::verifierDeplacementsReine(Insecte* reine, const std::vector<Hexagon>& ennemisVoisins, const std::map<Hexagon, Insecte*>& plateau) {
    if (ennemisVoisins.size() > 3) {
        std::vector<Hexagon> deplacementsReine = reine->deplacementsPossibles(plateau);
        std::vector<Hexagon> deplacementsValides;
        for (const Hexagon& deplacement : deplacementsReine) {
            // Si le déplacement éloigne la Reine de ses ennemis
            std::vector<Hexagon> nouveauxVoisins = getVoisins(deplacement);
            std::vector<Hexagon> nouveauxEnnemisVoisins = reine->getVoisinsEnnemis(nouveauxVoisins, plateau);
            if (nouveauxEnnemisVoisins.size() < ennemisVoisins.size()) {
                deplacementsValides.push_back(deplacement);
            }
        }
        if (!deplacementsValides.empty()) {
            candidats[reine] = deplacementsValides; // Mettre à jour les candidats avec la Reine
        }
    }
}


void JoueurIANiveau2::verifierDeplacementsAllies(Insecte* reine, const std::vector<Hexagon>& voisinsReine, const std::map<Hexagon, Insecte*>& plateau) {
    // Récupérer les alliés parmi les voisins de la Reine
    std::vector<Insecte*> voisinsAllies;
    for (const auto& voisin : voisinsReine) {
        auto it = plateau.find(voisin);
        if (it != plateau.end() && it->second != nullptr && it->second->getOwner() == reine->getOwner()) {
            voisinsAllies.push_back(it->second);
        }
    }

    // Vérifier les déplacements possibles pour chaque allié
    for (Insecte* allie : voisinsAllies) {
        std::vector<Hexagon> deplacementsPossibles = allie->deplacementsPossibles(plateau);
        std::vector<Hexagon> deplacementsValides;
        for (const Hexagon& deplacement : deplacementsPossibles) {
            // Si le déplacement éloigne l'insecte des voisins de la Reine
            if (std::find(voisinsReine.begin(), voisinsReine.end(), deplacement) == voisinsReine.end()) {
                deplacementsValides.push_back(deplacement);
            }
        }
        if (!deplacementsValides.empty()) {
            candidats[allie] = deplacementsValides; // Mettre à jour les candidats avec les alliés
        }
    }
}

int JoueurIANiveau2::findIndexInOptions(Insecte* insecteChoisi, const std::map<Insecte*, std::vector<Hexagon>>& candidats, const std::vector<Hexagon>& options) {
    if (!insecteChoisi) {
        throw std::runtime_error("Insecte choisi invalide !");
    }

    // Trouver les options possibles pour l'insecte choisi dans candidats
    auto it = candidats.find(insecteChoisi);
    if (it != candidats.end() && !it->second.empty()) {
        // Obtenir un index aléatoire dans les options possibles pour cet insecte
        int choixIndex = randomIndex(0, static_cast<int>(it->second.size() - 1));


        // Récupérer le Hexagon correspondant
        Hexagon choix = it->second[choixIndex];

        // Retrouver l'index de ce choix dans options
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == choix) {
                return static_cast<int>(i);
            }
        }
    }

    throw std::runtime_error("Aucune correspondance trouvée entre candidats et options !");
}

void JoueurIANiveau2::reinitialiserAttributs() {
    actionChoisie = AUCUN_ACTION;
    insecteChoisi = nullptr;
    positionChoisie = Hexagon();
    candidats.clear(); // Réinitialiser les candidats
}

void JoueurIANiveau2::afficherCandidats() const {
    std::cout << "Candidats actuels : ";
    if (candidats.empty()) {
        std::cout << "Aucun candidat trouvé." << std::endl;
        return;
    }

    for (const auto& pair : candidats) {
        std::cout << "Insecte : " << pair.first->getNom() << " Déplacements : ";
        for (const Hexagon& deplacement : pair.second) {
            std::cout << "(" << deplacement.getQ() << ", " << deplacement.getR() << ") ";
        }
        std::cout << std::endl;
    }
}


HeuristiqueType JoueurIANiveau2::choisirHeuristique(const std::map<Hexagon, Insecte*>& plateau) {
    reinitialiserAttributs(); // Réinitialiser les attributs au début de l'heuristique

    // Récupérer la Reine sur le plateau
    Insecte* reine = getQueenOnPlateau(plateau);
    if (reine) {
        // Vérifier si la Reine a plus de 3 voisins
        std::vector<Hexagon> voisinsReine = getVoisins(reine->getCoords());
        if (voisinsReine.size() > 3) {
            protegerReine(plateau);
            historiqueHeuristiques.push_back(PROTEGER_REINE);
            return PROTEGER_REINE;
        }
    }
    // Ajouter d'autres heuristiques ici si nécessaire

    historiqueHeuristiques.push_back(AUCUN_HEURISTIQUE);
    return AUCUN_HEURISTIQUE;
}

void JoueurIANiveau2::afficherHistoriqueHeuristiques() const {
    std::cout << "Historique des heuristiques utilisées : ";
    for (const auto& heuristique : historiqueHeuristiques) {
        switch (heuristique) {
            case PROTEGER_REINE:
                std::cout << "PROTEGER_REINE ";
                break;
            case ATTAQUER_REINE:
                std::cout << "ATTAQUER_REINE ";
                break;
            case COMPACTER_RUCHE:
                std::cout << "COMPACTER_RUCHE ";
                break;
            case AUCUN_HEURISTIQUE:
                std::cout << "AUCUN_HEURISTIQUE ";
                break;
        }
    }
    std::cout << std::endl;
}



int JoueurIANiveau2::getInputForAction(const std::map<Hexagon, Insecte*>& plateau) {
    // Appeler l'heuristique et récupérer l'action
    HeuristiqueType heuristiqueChoisie = choisirHeuristique(plateau);

    // Afficher l'historique des heuristiques
    afficherHistoriqueHeuristiques();
    afficherCandidats();

    // Si une action a été choisie, la retourner
    if (actionChoisie != AUCUN_ACTION) {
        return static_cast<int>(actionChoisie);
    }

    // Si aucune heuristique n'a fonctionné, retourner une action aléatoire
    return randomChoice();
}



int JoueurIANiveau2::getInputForDeckIndex() {
    if (candidats.empty()) {
        return randomDeckChoice(); // Fallback : choisir un insecte aléatoire dans le deck
    }

    // Choisir un insecte aléatoire dans candidats
    auto it = candidats.begin();
    std::advance(it, randomIndex(0, static_cast<int>(candidats.size() - 1)));
    insecteChoisi = it->first; // Définir l'insecte choisi

    // Utiliser la fonction externe pour retrouver l'index
    int index = findInsectIndexInDeck(getDeck(), insecteChoisi);
    if (index == -1) {
        throw std::runtime_error("Insecte choisi non trouvé dans le deck !");
    }
    return index;
}

int JoueurIANiveau2::getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) {
    if (candidats.empty() || !insecteChoisi) {
        return randomHexagonIndexChoice(placementsPossibles); // Fallback : choisir une position aléatoire
    }

    return findIndexInOptions(insecteChoisi, candidats, placementsPossibles);
}


int JoueurIANiveau2::getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) {
    if (candidats.empty()) {
        return randomPionIndexChoice(insectesDuJoueur); // Fallback : choisir un insecte aléatoire
    }

    // Choisir un insecte aléatoire parmi les candidats
    auto it = candidats.begin();
    std::advance(it, randomIndex(0, static_cast<int>(candidats.size() - 1)));
    insecteChoisi = it->first; // Définir l'insecte choisi

    // Trouver l'index de cet insecte dans insectesDuJoueur
    for (size_t i = 0; i < insectesDuJoueur.size(); ++i) {
        if (insectesDuJoueur[i] == insecteChoisi) {
            return static_cast<int>(i);
        }
    }

    throw std::runtime_error("Insecte choisi non trouvé dans insectesDuJoueur !");
}

int JoueurIANiveau2::getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) {
    if (candidats.empty() || !insecteChoisi) {
        return randomHexagonIndexChoice(deplacementsPossibles); // Fallback : choisir un mouvement aléatoire
    }

    return findIndexInOptions(insecteChoisi, candidats, deplacementsPossibles);
}











