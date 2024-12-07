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
Insecte* Insecte::trouverReine(Joueur* joueur, const std::map<Hexagon, Insecte*>& plateau) {
    for (auto it = plateau.begin(); it != plateau.end(); ++it) {
        const Hexagon& position = it->first;
        Insecte* insecte = it->second;
        if (insecte != nullptr && insecte->getOwner() == joueur && insecte->isQueen()) {
            return insecte;
        }
    }
    return nullptr;
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

HeuristiqueType JoueurIANiveau2::choisirHeuristique(Joueur* joueur, Joueur* adversaire, const std::map<Hexagon, Insecte*>& plateau) {
        // Trouver la Reine du joueur
    Insecte* reine = trouverReine(joueur, plateau);
    Insecte* reineAdverse = trouverReine(adversaire, plateau);

    if (reine == nullptr || reineAdverse == nullptr) {
        return AUCUN_HEURISTIQUE;
    }

    // Récupérer les voisins de la Reine
    std::vector<Hexagon> voisinsReine = getVoisins(reine->getCoords());
    std::vector<Hexagon> ennemisVoisins = reine->getVoisinsEnnemis(voisinsReine, plateau);

    // Si la Reine est en danger (trop de voisins ennemis), choisir de la protéger
    if (ennemisVoisins.size() > 3) {
        return PROTEGER_REINE;
    }

// Vérifier si l'un des insectes alliés peut atteindre la Reine adverse
    for (auto it = plateau.begin(); it != plateau.end(); ++it) {
        Insecte* insecte = it->second;

        if (insecte != nullptr && insecte->getOwner() == joueur) {
            std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateau);
            if (std::find(deplacementsPossibles.begin(), deplacementsPossibles.end(), reineAdverse->getCoords()) != deplacementsPossibles.end()) {
                return ATTAQUER_REINE;
            }
        }
    }


    // Sinon, jouer de manière défensive pour compacter la ruche
    return COMPACTER_RUCHE;
}
void JoueurIANiveau2::choisirAction(std::map<Hexagon, Insecte*>& plateau) {
        // Supposons que vous ayez des pointeurs vers le joueur et l'adversaire
    Joueur* joueur = this;
    Joueur* adversaire = nullptr; // Initialiser correctement l'adversaire

    // Choisir l'heuristique en utilisant la fonction déterministe
    HeuristiqueType heuristique = choisirHeuristique(joueur, adversaire, plateau);

    // Ensuite, appliquer la logique associée à l'heuristique choisie
    switch (heuristique) {
        case PROTEGER_REINE:
            deplacerPourProtegerReine(trouverReine(joueur, plateau), plateau);
            break;
        case ATTAQUER_REINE:
            // Implémenter la logique d'attaque de la Reine adverse
            break;
        case COMPACTER_RUCHE:
            // Implémenter la logique pour compacter la ruche
            break;
        default:
            // Si aucune heuristique particulière ne s'applique, faire un coup aléatoire
            //defaultAction(plateau);
            break;
    }
}

void JoueurIANiveau2::deplacerPourProtegerReine(Insecte* reine, std::map<Hexagon, Insecte*>& plateau) {
    // Récupérer les voisins de la Reine et déterminer les ennemis
    std::vector<Hexagon> voisinsReine = getVoisins(reine->getCoords());
    std::vector<Hexagon> ennemisVoisins = reine->getVoisinsEnnemis(voisinsReine, plateau);

    // Vérifier si la Reine elle-même peut être déplacée pour réduire le nombre de voisins ennemis
    if (ennemisVoisins.size() > 3) {
        std::vector<Hexagon> deplacementsReine = reine->deplacementsPossibles(plateau);
        for (const Hexagon& deplacement : deplacementsReine) {
            // Si le déplacement éloigne la Reine de ses ennemis
            std::vector<Hexagon> nouveauxVoisins = getVoisins(deplacement);
            std::vector<Hexagon> nouveauxEnnemisVoisins = reine->getVoisinsEnnemis(nouveauxVoisins, plateau);
            if (nouveauxEnnemisVoisins.size() < ennemisVoisins.size()) {
                actionChoisie = DEPLACER;
                insecteChoisi = reine;
                positionChoisie = deplacement;
                return;
            }
        }
    }

    // Compter le nombre de voisins alliés
    std::vector<Insecte*> voisinsAllies;
    for (const auto& voisin : voisinsReine) {
        auto it = plateau.find(voisin);
        if (it != plateau.end() && it->second != nullptr) {
            if (it->second->getOwner() == reine->getOwner()) {
                voisinsAllies.push_back(it->second);
            }
        }
    }

    // Tenter de déplacer un allié pour protéger la Reine
    for (Insecte* allie : voisinsAllies) {
        std::vector<Hexagon> deplacementsPossibles = allie->deplacementsPossibles(plateau);
        for (const Hexagon& deplacement : deplacementsPossibles) {
            // Si le déplacement éloigne l'insecte des voisins de la Reine
            if (std::find(voisinsReine.begin(), voisinsReine.end(), deplacement) == voisinsReine.end()) {
                // Déterminer le déplacement
                actionChoisie = DEPLACER;
                insecteChoisi = allie;
                positionChoisie = deplacement;
                return;  // Un seul mouvement suffit
            }
        }
    }

    // Si aucune action n'a été trouvée
    actionChoisie = AUCUN_ACTION;
    insecteChoisi = nullptr;
}


Hexagon JoueurIANiveau2::getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour){
    //A implémenter si on veut faire commencer IA ou faire jouer IA contre IA
    return Hexagon(0,0);
}
