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

Insecte* Joueur::getReineAdverse(const std::map<Hexagon, Insecte*>& plateau) const {
    for (const auto& [position, insecte] : plateau) {
        if (insecte != nullptr && insecte->isQueen() && insecte->getOwner() != this) {
            return insecte; // Retourner la Reine adverse trouvée
        }
    }
    return nullptr; // Pas de Reine adverse trouvée
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

void JoueurIANiveau2::intersectionCandidats() {
    if (candidats.empty()) {
        candidats = std::move(nouveauxCandidats); // Si candidats est vide, utiliser les nouveaux candidats
    } else {
        std::map<Insecte*, std::vector<Hexagon>> intersection;

        for (const auto& [insecte, deplacements] : candidats) {
            if (nouveauxCandidats.count(insecte)) {
                std::vector<Hexagon> intersectionDeplacements;
                const auto& nouveauxDeplacements = nouveauxCandidats.at(insecte);

                // Trouver l'intersection des déplacements
                std::set_intersection(
                    deplacements.begin(), deplacements.end(),
                    nouveauxDeplacements.begin(), nouveauxDeplacements.end(),
                    std::back_inserter(intersectionDeplacements)
                );

                // Ajouter l'insecte si l'intersection n'est pas vide
                if (!intersectionDeplacements.empty()) {
                    intersection[insecte] = std::move(intersectionDeplacements);
                }
            }
        }

        // Si l'intersection est vide, conserver les anciens candidats
        if (!intersection.empty()) {
            candidats = std::move(intersection);
        }
    }

    nouveauxCandidats.clear(); // Réinitialiser les nouveaux candidats après utilisation
}



void JoueurIANiveau2::protegerReine() {
    // Récupérer la Reine du joueur sur le plateau
    Insecte* reine = getQueenOnPlateau(getPlateau());
    if (!reine) {
        return;
    }

    // Récupérer les voisins de la Reine et déterminer les ennemis
    std::vector<Hexagon> voisinsReine = getVoisins(reine->getCoords());
    std::vector<Hexagon> ennemisVoisins = reine->getVoisinsEnnemis(voisinsReine, getPlateau());

    // Vérifier s'il y a moins de 3 ennemis autour de la Reine
    if (ennemisVoisins.size() < 3) {
        nouveauxCandidats.clear(); // Laisser nouveauxCandidats vide
        return;
    }

    // Ajouter les déplacements possibles de la Reine
    nouveauxCandidats.clear(); // Réinitialiser pour cette heuristique
    verifierDeplacementsReine(reine, ennemisVoisins);

    // Ajouter les déplacements possibles des alliés
    verifierDeplacementsAllies(reine, voisinsReine);

    // Filtrer ou joindre les candidats avec les nouveaux
    intersectionCandidats();
}


void JoueurIANiveau2::attaquerReine() {
    // Récupérer la Reine adverse via Joueur::getReineAdverse
    Insecte* reineAdverse = getReineAdverse(getPlateau());
    if (!reineAdverse) {
        return; // Pas de Reine adverse sur le plateau, rien à faire
    }

    // Récupérer les voisins de la Reine adverse
    std::vector<Hexagon> voisinsReineAdverse = getVoisins(reineAdverse->getCoords());

    // Réinitialiser nouveauxCandidats
    nouveauxCandidats.clear();

    // Parcourir tous les insectes alliés sur le plateau
    for (const auto& [position, insecte] : getPlateau()) {
        if (insecte->getOwner() == this) {
            std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(getPlateau());

            // Filtrer les déplacements pour ne garder que ceux dans les voisins de la Reine adverse
            std::vector<Hexagon> deplacementsVersReineAdverse;
            for (const Hexagon& deplacement : deplacementsPossibles) {
                if (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), deplacement) != voisinsReineAdverse.end()) {
                    deplacementsVersReineAdverse.push_back(deplacement);
                }
            }

            // Si des déplacements valides existent, ajouter l'insecte et ses déplacements à nouveauxCandidats
            if (!deplacementsVersReineAdverse.empty()) {
                nouveauxCandidats[insecte] = std::move(deplacementsVersReineAdverse);
            }
        }
    }

    // Appeler la fonction pour faire l'intersection avec les candidats existants
    intersectionCandidats();
}


void JoueurIANiveau2::verifierDeplacementsReine(Insecte* reine, const std::vector<Hexagon>& ennemisVoisins) {
    if (ennemisVoisins.size() > 3) {
        std::vector<Hexagon> deplacementsReine = reine->deplacementsPossibles(getPlateau());
        std::vector<Hexagon> deplacementsValides;
        for (const Hexagon& deplacement : deplacementsReine) {
            // Si le déplacement éloigne la Reine de ses ennemis
            std::vector<Hexagon> nouveauxVoisins = getVoisins(deplacement);
            std::vector<Hexagon> nouveauxEnnemisVoisins = reine->getVoisinsEnnemis(nouveauxVoisins, getPlateau());
            if (nouveauxEnnemisVoisins.size() < ennemisVoisins.size()) {
                deplacementsValides.push_back(deplacement);
            }
        }
        if (!deplacementsValides.empty()) {
            nouveauxCandidats[reine] = deplacementsValides; // Mettre à jour les candidats avec la Reine
        }
    }
}

void JoueurIANiveau2::remplirCandidatsAvecDeck() {
    candidats.clear(); // Réinitialiser les candidats

    // Ajouter chaque insecte du deck à candidats avec les mêmes placements possibles
    for (Insecte* insecte : getDeck()) {
        candidats[insecte] = insecte->getPlacementsPossibles(getPlateau());
    }
}



void JoueurIANiveau2::verifierDeplacementsAllies(Insecte* reine, const std::vector<Hexagon>& voisinsReine) {
    // Récupérer les alliés parmi les voisins de la Reine
    std::vector<Insecte*> voisinsAllies;
    for (const auto& voisin : voisinsReine) {
        auto it = getPlateau().find(voisin);
        if (it != getPlateau().end() && it->second != nullptr && it->second->getOwner() == reine->getOwner()) {
            voisinsAllies.push_back(it->second);
        }
    }

    // Vérifier les déplacements possibles pour chaque allié
    for (Insecte* allie : voisinsAllies) {
        std::vector<Hexagon> deplacementsPossibles = allie->deplacementsPossibles(getPlateau());
        std::vector<Hexagon> deplacementsValides;
        for (const Hexagon& deplacement : deplacementsPossibles) {
            // Si le déplacement éloigne l'insecte des voisins de la Reine
            if (std::find(voisinsReine.begin(), voisinsReine.end(), deplacement) == voisinsReine.end()) {
                deplacementsValides.push_back(deplacement);
            }
        }
        if (!deplacementsValides.empty()) {
            nouveauxCandidats[allie] = deplacementsValides; // Mettre à jour les candidats avec les alliés
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
                return static_cast<int>(i+1);
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
    nouveauxCandidats.clear();
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

void JoueurIANiveau2::filtrerPrioriteFourmies() {
    // Vérifier s'il y a des fourmies dans les candidats
    bool aDesFourmies = false;
    for (const auto& [insecte, placements] : candidats) {
        if (insecte->getNom() == "Fourmi") {
            aDesFourmies = true;
            break;
        }
    }

    // Si aucune fourmi, ne rien faire
    if (!aDesFourmies) {
        return;
    }

    // Générer une probabilité pour prioriser les fourmies
    float chance = randomFloat(0.0f, 1.0f); // Fonction utilitaire pour générer un float entre 0 et 1
    if (chance <= 0.75f) {
        // Garder uniquement les fourmies dans les candidats
        std::map<Insecte*, std::vector<Hexagon>> nouveauxCandidats;

        for (const auto& [insecte, placements] : candidats) {
            if (insecte->getNom() == "Fourmi") {
                nouveauxCandidats[insecte] = placements;
            }
        }

        candidats = std::move(nouveauxCandidats); // Remplacer les candidats par ceux filtrés
        nouveauxCandidats.clear();
    }
}



void JoueurIANiveau2::choisirHeuristiquePourDeplacer() {

    // Appliquer les heuristiques de déplacement
    protegerReine();
    attaquerReine();

    // Prioriser les candidats
    if (!candidats.empty()) {
        actionChoisie = DEPLACER;
    }
}


void JoueurIANiveau2::choisirHeuristiquePourPlacer() {
    remplirCandidatsAvecDeck();
    filtrerPrioriteFourmies();
    // Filtrer ou prioriser les candidats selon les heuristiques (par exemple, proximité des ennemis)
    if (!candidats.empty()) {
        actionChoisie = PLACER;
    }
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



int JoueurIANiveau2::getInputForAction() {
    candidats.clear(); // Réinitialiser les candidats

    // Calculer la phase du jeu
    int tour = getTour();
    float probaPlacer = 0.0, probaDeplacer = 0.0;

    if (tour <= 6 || getDeckSize() > 0.75 * tailleDeckInitiale()) {
        // Début de jeu
        probaPlacer = 0.8;
        probaDeplacer = 0.2;
    } else if (tour <= 12 || (getDeckSize() > 0.25 * tailleDeckInitiale())) {
        // Milieu de jeu
        probaPlacer = 0.5;
        probaDeplacer = 0.5;
    } else {
        // Fin de jeu
        probaPlacer = 0.3;
        probaDeplacer = 0.7;
    }

    // Générer une action basée sur les probabilités
    float choix = randomFloat();
    if (choix < probaPlacer) {
        choisirHeuristiquePourPlacer();
    } else {
        choisirHeuristiquePourDeplacer();
    }

    // Afficher les informations pour le débogage
    afficherHistoriqueHeuristiques();
    afficherCandidats();

    // Retourner l'action choisie si disponible
    if (actionChoisie != AUCUN_ACTION) {
        return static_cast<int>(actionChoisie);
    }

    // Si aucune action n'a été trouvée, retourner une action aléatoire
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











