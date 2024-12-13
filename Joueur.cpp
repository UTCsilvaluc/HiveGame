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

Insecte* Joueur::getReineAdverse(const std::map<Hexagon, Insecte*>& plateauSimule) const {
    for (const auto& [position, insecte] : plateauSimule) {
        if (insecte && insecte->getOwner() != this && insecte->isQueen()) {
            return insecte;
        }
    }
    return nullptr; // Retourne null si la reine adverse n'est pas trouvée
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

Insecte* Joueur::contientInsecte(const std::string& nomInsecte) const {
    for (Insecte* insecte : deck) {
        if (insecte->getNom() == nomInsecte) {
            return insecte;
        }
    }
    return nullptr;
}

void Joueur::ajouterInsecte(Insecte* insecte) {
    if (insecte == nullptr) {
        std::cerr << "Erreur : Impossible d'ajouter un insecte nul au deck." << std::endl;
        return;
    }
    deck.push_back(insecte);
}

std::vector<Insecte*> Joueur::getInsectesDuJoueur(const std::map<Hexagon, Insecte*>& plateauMap) const {
    std::vector<Insecte*> insectesDuJoueur;
    for (const auto& pair : plateauMap) {
        Insecte* insecte = pair.second;
        if (insecte->getOwner() == this) {
            insectesDuJoueur.push_back(insecte);
        }
    }
    return insectesDuJoueur;
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

void JoueurIANiveau2::remplirCandidatsAvecDeck(const Joueur* joueur,
                                               const std::vector<Insecte*>& deck,
                                               const std::map<Hexagon, Insecte*>& plateauSimule) {
    candidats.clear(); // Réinitialiser les candidats

    // Utiliser un ensemble pour éviter les doublons basés sur le nom des insectes
    std::set<std::string> insectesTraites;

    for (Insecte* insecte : deck) {
        // Vérifier si l'insecte (par son nom) a déjà été traité
        if (insectesTraites.count(insecte->getNom()) > 0) {
            continue; // Insecte déjà traité
        }
        insectesTraites.insert(insecte->getNom());

        // Calculer les placements possibles en utilisant le plateau simulé
        std::vector<Hexagon> placements = insecte->getPlacementsPossibles(plateauSimule);

        // Ajouter aux candidats si des placements sont possibles
        if (!placements.empty()) {
            candidats[insecte] = std::move(placements);
        }
    }
}





int JoueurIANiveau2::findIndexInOptions(const Joueur* joueur, Insecte* insecteChoisi, const std::map<Insecte*, std::vector<Hexagon>>& candidats, const std::vector<Hexagon>& options) {
    (void)joueur; // Si pas utilisé ici, on peut l'ignorer ou l'utiliser si besoin
    if (!insecteChoisi) {
        throw std::runtime_error("Insecte choisi invalide !");
    }

    auto it = candidats.find(insecteChoisi);
    if (it != candidats.end() && !it->second.empty()) {
        int choixIndex = randomIndex(0, static_cast<int>(it->second.size() - 1));
        Hexagon choix = it->second[choixIndex];

        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == choix) {
                reinitialiserAttributs();
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
    candidats.clear();
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

int JoueurIANiveau2::evaluerCohesion(const Joueur* joueur, const Hexagon& emplacement,
                                     const std::map<Hexagon, Insecte*>& plateauSimule) const {
    std::vector<Hexagon> voisins = getVoisins(emplacement);
    int score = 0;

    // Parcourir les voisins pour évaluer leur cohésion
    for (const Hexagon& voisin : voisins) {
        auto it = plateauSimule.find(voisin);
        if (it != plateauSimule.end() && it->second && it->second->getOwner() == joueur) {
            score++;
        }
    }

    return score;
}


int JoueurIANiveau2::evaluerPlacementAction(const Joueur* joueur, Insecte* insecte, const Hexagon& emplacement,
                                            const std::map<Hexagon, Insecte*>& plateauSimule) const {
    // Simuler le placement de l'insecte sur le plateau simulé
    std::map<Hexagon, Insecte*> plateauCopie = plateauSimule;
    plateauCopie[emplacement] = insecte;

    // Évaluer la cohésion avec le plateau simulé
    int score = evaluerCohesion(joueur, emplacement, plateauCopie);

    // Obtenir les déplacements possibles de l'insecte avec le plateau simulé
    std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateauCopie);

    // Calculer le meilleur score de déplacement
    int meilleurScoreDeplacement = std::numeric_limits<int>::min();
    for (const Hexagon& dep : deplacementsPossibles) {
        int scoreDeplacement = evaluerDeplacementAction(joueur, insecte, dep, plateauCopie);
        meilleurScoreDeplacement = std::max(meilleurScoreDeplacement, scoreDeplacement);
    }

    // Si des déplacements sont possibles, ajouter leur potentiel futur au score
    if (!deplacementsPossibles.empty()) {
        int potentielFutur = meilleurScoreDeplacement + static_cast<int>(deplacementsPossibles.size());
        score += static_cast<int>(0.75f * potentielFutur);
    }

    // Retourner le score final pondéré
    return static_cast<int>(0.75f * score);
}


int JoueurIANiveau2::evaluerAttaqueReineAdverse(const Joueur* joueur, Insecte* insecte,
                                                const Hexagon& nouvelEmplacement,
                                                const std::map<Hexagon, Insecte*>& plateauSimule) const {
    int score = 0;

    // Obtenir la reine adverse à partir du plateau simulé
    Insecte* reineAdverse = joueur->getReineAdverse(plateauSimule);

    if (reineAdverse) {
        // Obtenir la position de la reine adverse à partir du plateau simulé
        Hexagon positionReineAdverse = getInsectPosition(plateauSimule, reineAdverse);

        // Obtenir les voisins de la reine adverse
        std::vector<Hexagon> voisinsReineAdverse = getVoisins(positionReineAdverse);

        // Obtenir la position actuelle de l'insecte sur le plateau simulé
        Hexagon positionActuelle = getInsectPosition(plateauSimule, insecte);

        // Vérifier les menaces avant et après le déplacement
        bool dejaMenacant = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), positionActuelle) != voisinsReineAdverse.end());
        bool menacantApres = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), nouvelEmplacement) != voisinsReineAdverse.end());

        // Ajuster le score en fonction des menaces
        if (menacantApres && !dejaMenacant) {
            score += 10; // Bonus pour une nouvelle menace
        }
        if (dejaMenacant && !menacantApres) {
            score -= 5; // Malus si l'insecte cesse de menacer la reine
        }
    }

    return score;
}


int JoueurIANiveau2::evaluerProtectionReine(const Joueur* joueur, Insecte* insecte,
                                            const Hexagon& ancienneEmplacement, const Hexagon& nouvelEmplacement,
                                            const std::map<Hexagon, Insecte*>& plateauSimule) const {
    int score = 0;

    // Obtenir la reine alliée à partir du plateau simulé
    Insecte* reineAlliee = joueur->getQueenOnPlateau(plateauSimule);
    if (!reineAlliee) return score;

    // Récupérer la position de la reine alliée sur le plateau simulé
    Hexagon positionReineAlliee = getInsectPosition(plateauSimule, reineAlliee);

    // Obtenir les voisins de la reine alliée avant la simulation
    std::vector<Hexagon> voisinsReineAvant = getVoisins(positionReineAlliee);
    std::vector<Hexagon> ennemisAvant = reineAlliee->getVoisinsEnnemis(voisinsReineAvant, plateauSimule);
    size_t nbEnnemisAvant = ennemisAvant.size();

    // Simuler le déplacement de l'insecte
    std::map<Hexagon, Insecte*> plateauModifie = plateauSimule;
    plateauModifie.erase(ancienneEmplacement);
    plateauModifie[nouvelEmplacement] = insecte;

    // Obtenir la reine alliée simulée sur le plateau modifié
    Insecte* reineAllieeSimulee = joueur->getQueenOnPlateau(plateauModifie);
    if (!reineAllieeSimulee) return score;

    // Récupérer les voisins de la reine alliée après la simulation
    Hexagon positionReineAllieeApres = getInsectPosition(plateauModifie, reineAllieeSimulee);
    std::vector<Hexagon> voisinsReineApres = getVoisins(positionReineAllieeApres);
    std::vector<Hexagon> ennemisApres = reineAllieeSimulee->getVoisinsEnnemis(voisinsReineApres, plateauModifie);
    size_t nbEnnemisApres = ennemisApres.size();

    // Calculer la différence entre les ennemis avant et après
    int difference = (int)nbEnnemisAvant - (int)nbEnnemisApres;

    // Ajuster le score en fonction de la différence
    if (difference > 0) {
        float multiplicateur = 1.0f;
        if (nbEnnemisAvant > 4) {
            multiplicateur = 1.5f;
        }

        int baseBonus = 0;
        if (insecte == reineAlliee) {
            baseBonus = 10 * difference;
        } else {
            baseBonus = 5 * difference;
        }

        score += (int)(baseBonus * multiplicateur);
    } else if (difference < 0) {
        score -= 5 * (-difference);
    }

    return score;
}


int JoueurIANiveau2::evaluerBlocageInsecteImportant(const Joueur* joueur, Insecte* insecte,
                                                    const Hexagon& anciennePos, const Hexagon& nouvelEmplacement,
                                                    const std::map<Hexagon, Insecte*>& plateauSimule) const {
    int bonusBlocage = 7;

    // Obtenir les voisins du nouvel emplacement
    std::vector<Hexagon> voisins = getVoisins(nouvelEmplacement);

    // Identifier les insectes importants adverses à proximité
    std::vector<Insecte*> insectesImportants;
    for (const Hexagon& voisin : voisins) {
        auto it = plateauSimule.find(voisin);
        if (it != plateauSimule.end() && it->second != nullptr) {
            Insecte* adv = it->second;
            if (adv->getOwner() != joueur && (adv->isQueen() || adv->getNom() == "Fourmi")) {
                // Vérifier si l'insecte important peut se déplacer avant le blocage
                std::vector<Hexagon> deplacementsAvant = adv->deplacementsPossibles(plateauSimule);
                if (!deplacementsAvant.empty()) {
                    insectesImportants.push_back(adv);
                }
            }
        }
    }

    // Si aucun insecte important n'est affecté, retourner 0
    if (insectesImportants.empty()) {
        return 0;
    }

    // Simuler le plateau avec le déplacement de l'insecte
    std::map<Hexagon, Insecte*> plateauModifie = plateauSimule;
    plateauModifie.erase(anciennePos);
    plateauModifie[nouvelEmplacement] = insecte;

    // Calculer le bonus de blocage
    int totalBonus = 0;
    for (Insecte* adv : insectesImportants) {
        std::vector<Hexagon> deplacementsApres = adv->deplacementsPossibles(plateauModifie);
        if (deplacementsApres.empty()) {
            totalBonus += bonusBlocage;
        }
    }

    return totalBonus;
}


int JoueurIANiveau2::evaluerDeplacementAction(const Joueur* joueur, Insecte* insecte,
                                              const Hexagon& nouvelEmplacement,
                                              const std::map<Hexagon, Insecte*>& plateauSimule) const {
    // Obtenir la position actuelle de l'insecte via le plateau simulé
    Hexagon anciennePos = getInsectPosition(plateauSimule, insecte);

    int scoreTotal = 0;
    scoreTotal += evaluerAttaqueReineAdverse(joueur, insecte, nouvelEmplacement, plateauSimule);
    scoreTotal += evaluerProtectionReine(joueur, insecte, anciennePos, nouvelEmplacement, plateauSimule);
    scoreTotal += evaluerBlocageInsecteImportant(joueur, insecte, anciennePos, nouvelEmplacement, plateauSimule);

    return scoreTotal;
}



std::vector<std::pair<Hexagon, int>> JoueurIANiveau2::evaluerEtTrierMouvements(const Joueur* joueur, Insecte* insecte,
                                                                              const std::vector<Hexagon>& options,
                                                                              bool estPlacement,
                                                                              const std::map<Hexagon, Insecte*>& plateauSimule) const {
    std::vector<std::pair<Hexagon, int>> mouvementsAvecScore;

    for (const Hexagon& option : options) {
        int score = estPlacement ?
                    evaluerPlacementAction(joueur, insecte, option, plateauSimule) :
                    evaluerDeplacementAction(joueur, insecte, option, plateauSimule);
        mouvementsAvecScore.emplace_back(option, score);
    }

    // Trier les mouvements par score décroissant
    std::sort(mouvementsAvecScore.begin(), mouvementsAvecScore.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    return mouvementsAvecScore;
}


std::vector<Hexagon> JoueurIANiveau2::extraireMeilleursMouvements(const std::vector<std::pair<Hexagon, int>>& mouvementsTries,
                                                                  int nombreMax) const {
    std::vector<Hexagon> meilleurs;

    // Extraire jusqu'à "nombreMax" meilleurs mouvements
    for (int i = 0; i < std::min(static_cast<int>(mouvementsTries.size()), nombreMax); ++i) {
        meilleurs.push_back(mouvementsTries[i].first);
    }

    return meilleurs;
}


int JoueurIANiveau2::calculerScoreMaxParInsecte(const Joueur* joueur, Insecte* insecte,
                                                bool estPlacement,
                                                const std::map<Hexagon, Insecte*>& plateauSimule) const {
    // Vérifier si l'insecte est dans les nouveaux candidats
    auto it = nouveauxCandidats.find(insecte);
    if (it == nouveauxCandidats.end() || it->second.empty()) {
        return std::numeric_limits<int>::min(); // Aucun candidat pour cet insecte
    }

    // Calculer le meilleur score parmi les options disponibles
    int meilleurScore = std::numeric_limits<int>::min();
    for (const Hexagon& option : it->second) {
        int score = estPlacement ?
                    evaluerPlacementAction(joueur, insecte, option, plateauSimule) :
                    evaluerDeplacementAction(joueur, insecte, option, plateauSimule);
        meilleurScore = std::max(meilleurScore, score);
    }

    return meilleurScore;
}


void JoueurIANiveau2::filtrerMeilleursInsectes(const Joueur* joueur,
                                               std::map<Insecte*, std::vector<Hexagon>>& insectesCandidats,
                                               int nombreMeilleursInsectes, bool estPlacement,
                                               const std::map<Hexagon, Insecte*>& plateauSimule) {
    if (insectesCandidats.empty()) return;

    // Calculer les scores pour chaque insecte
    std::vector<std::pair<Insecte*, int>> insectesAvecScore;
    for (const auto& [insecte, _] : insectesCandidats) {
        int scoreMax = calculerScoreMaxParInsecte(joueur, insecte, estPlacement, plateauSimule);
        insectesAvecScore.emplace_back(insecte, scoreMax);
    }

    // Trier les insectes par score décroissant
    std::sort(insectesAvecScore.begin(), insectesAvecScore.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    // Garder les meilleurs insectes
    std::map<Insecte*, std::vector<Hexagon>> meilleursInsectes;
    for (int i = 0; i < std::min(static_cast<int>(insectesAvecScore.size()), nombreMeilleursInsectes); ++i) {
        Insecte* insecte = insectesAvecScore[i].first;
        meilleursInsectes[insecte] = insectesCandidats[insecte];
    }

    // Remplacer les insectes candidats par les meilleurs
    insectesCandidats = std::move(meilleursInsectes);
}



void JoueurIANiveau2::remplirCandidatsAvecPlateau(const Joueur* joueur,
                                                  const std::map<Hexagon, Insecte*>& plateauSimule) {
    candidats.clear();

    // Parcourir le plateau simulé pour trouver les insectes du joueur
    for (const auto& [position, insecte] : plateauSimule) {
        if (insecte && insecte->getOwner() == joueur) {
            std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateauSimule);
            if (!deplacementsPossibles.empty()) {
                candidats[insecte] = std::move(deplacementsPossibles);
            }
        }
    }
}


void JoueurIANiveau2::choisirHeuristiquePourDeplacer(const Joueur* joueur,
                                                     const std::map<Hexagon, Insecte*>& plateauSimule) {
    remplirCandidatsAvecPlateau(joueur, plateauSimule);

    int nombreMaxDeplacements = 1;
    nouveauxCandidats.clear();

    for (const auto& [insecte, deplacements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(joueur, insecte, deplacements, false, plateauSimule);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxDeplacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    filtrerMeilleursInsectes(joueur, nouveauxCandidats, 2, false, plateauSimule);

    candidats = std::move(nouveauxCandidats);

    if (!candidats.empty()) {
        actionChoisie = DEPLACER;
    }
}


void JoueurIANiveau2::choisirHeuristiquePourPlacer(const Joueur* joueur,
                                                   const std::vector<Insecte*>& deck,
                                                   const std::map<Hexagon, Insecte*>& plateauSimule) {
    // Utiliser le deck fourni pour remplir les candidats
    remplirCandidatsAvecDeck(joueur, deck, plateauSimule);

    int nombreMaxPlacements = 1; // Nombre maximum de placements à considérer
    nouveauxCandidats.clear();

    // Parcourir les candidats pour évaluer et trier les placements
    for (const auto& [insecte, placements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(joueur, insecte, placements, true, plateauSimule);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxPlacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    // Filtrer les meilleurs insectes selon leur score
    filtrerMeilleursInsectes(joueur, nouveauxCandidats, 2, true, plateauSimule);

    // Mettre à jour les candidats avec les meilleurs résultats
    candidats = std::move(nouveauxCandidats);

    // Définir l'action choisie si des candidats sont trouvés
    if (!candidats.empty()) {
        actionChoisie = PLACER;
    }
}



double JoueurIANiveau2::calculerScoreMoyenDePlacement(const Joueur* joueur,
                                                      const std::map<Insecte*, std::vector<Hexagon>>& candidats,
                                                      const std::map<Hexagon, Insecte*>& plateauSimule) const {
    if (candidats.empty()) {
        return 0.0;
    }

    double somme = 0.0;
    int count = 0;

    for (const auto& [insecte, emplacements] : candidats) {
        for (const Hexagon& emplacement : emplacements) {
            int score = evaluerPlacementAction(joueur, insecte, emplacement, plateauSimule);
            somme += score;
            ++count;
        }
    }

    return (count > 0) ? (somme / count) : 0.0;
}


double JoueurIANiveau2::calculerScoreMoyenDeDeplacement(const Joueur* joueur,
                                                        const std::map<Insecte*, std::vector<Hexagon>>& candidats,
                                                        const std::map<Hexagon, Insecte*>& plateauSimule) const {
    if (candidats.empty()) {
        return 0.0;
    }

    double somme = 0.0;
    int count = 0;

    for (const auto& [insecte, deplacements] : candidats) {
        for (const Hexagon& deplacement : deplacements) {
            int score = evaluerDeplacementAction(joueur, insecte, deplacement, plateauSimule);
            somme += score;
            ++count;
        }
    }

    return (count > 0) ? (somme / count) : 0.0;
}



int JoueurIANiveau2::getInputForAction() {
    reinitialiserAttributs();

    // Évaluer les placements
    choisirHeuristiquePourPlacer(this, this->getDeck(), getPlateau());
    double scoreMoyenPlacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenPlacement = calculerScoreMoyenDePlacement(this, candidats, getPlateau());
    }
    std::map<Insecte*, std::vector<Hexagon>> candidatsPlacement = candidats;

    // Réinitialiser et évaluer les déplacements
    reinitialiserAttributs();
    choisirHeuristiquePourDeplacer(this, getPlateau());
    double scoreMoyenDeplacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenDeplacement = calculerScoreMoyenDeDeplacement(this, candidats, getPlateau());
    }

    // Décider de l'action en fonction des scores moyens
    if (scoreMoyenPlacement > scoreMoyenDeplacement) {
        candidats = candidatsPlacement;
        actionChoisie = PLACER;
    } else if (scoreMoyenDeplacement > 0.0) {
        actionChoisie = DEPLACER;
    } else {
        actionChoisie = (randomFloat() < 0.5) ? PLACER : DEPLACER;
    }

    return static_cast<int>(actionChoisie);
}



int JoueurIANiveau2::getInputForDeckIndex() {
    if (candidats.empty()) {
        return randomDeckChoice();
    }

    auto it = candidats.begin();
    std::advance(it, randomIndex(0, static_cast<int>(candidats.size() - 1)));
    insecteChoisi = it->first;

    int index = findInsectIndexInDeck(this->getDeck(), insecteChoisi);
    if (index == -1) {
        throw std::runtime_error("Insecte choisi non trouvé dans le deck !");
    }
    return index;
}

int JoueurIANiveau2::getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) {
    if (candidats.empty() || !insecteChoisi) {
        return randomHexagonIndexChoice(placementsPossibles);
    }
    return findIndexInOptions(this, insecteChoisi, candidats, placementsPossibles);
}


int JoueurIANiveau2::getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) {
    if (candidats.empty()) {
        return randomPionIndexChoice(insectesDuJoueur);
    }

    auto it = candidats.begin();
    std::advance(it, randomIndex(0, static_cast<int>(candidats.size() - 1)));
    insecteChoisi = it->first;

    for (size_t i = 0; i < insectesDuJoueur.size(); ++i) {
        if (insectesDuJoueur[i] == insecteChoisi) {
            return static_cast<int>(i);
        }
    }

    throw std::runtime_error("Insecte choisi non trouvé dans insectesDuJoueur !");
}

int JoueurIANiveau2::getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) {
    if (candidats.empty() || !insecteChoisi) {
        return randomHexagonIndexChoice(deplacementsPossibles);
    }

    return findIndexInOptions(this, insecteChoisi, candidats, deplacementsPossibles);
}

std::map<Hexagon, Insecte*> JoueurIANiveau3::simulerCoup(
    const std::map<Hexagon, Insecte*>& plateauSimule,
    const std::pair<Insecte*, Hexagon>& coup,
    std::vector<Insecte*>& deckMaximisateur,
    std::vector<Insecte*>& deckMinimisateur,
    bool estMaximisant) {

    // Copier le plateau simulé pour effectuer les modifications
    std::map<Hexagon, Insecte*> plateauModifie = plateauSimule;
    Insecte* insecte = coup.first;
    const Hexagon& nouvellePosition = coup.second;

    // Vérifier si l'action est un placement
    bool estUnPlacement = estPlacement(plateauModifie, insecte);

    if (estUnPlacement) {
        // Ajouter l'insecte à la nouvelle position sur le plateau
        plateauModifie[nouvellePosition] = insecte;

        // Obtenir le deck correspondant (maximisateur ou minimisateur)
        std::vector<Insecte*>& deckActuel = estMaximisant ? deckMaximisateur : deckMinimisateur;

        // Rechercher l'insecte dans le deck et le retirer
        auto it = std::find_if(deckActuel.begin(), deckActuel.end(),
                               [&insecte](Insecte* i) { return i->getNom() == insecte->getNom(); });

        if (it != deckActuel.end()) {
            deckActuel.erase(it);
        } else {
            std::cerr << "Erreur : Insecte non trouvé dans le deck pour le placement. Nom : " << insecte->getNom() << std::endl;
            for (const auto& insecteDeck : deckActuel) {
                std::cerr << " - " << insecteDeck->getNom() << std::endl;
            }
            throw std::runtime_error("Insecte non trouvé dans le deck pour le placement.");
        }

    } else {
        // Déplacement d'un insecte déjà placé
        Hexagon anciennePosition = getInsectPosition(plateauModifie, insecte);

        // Retirer l'insecte de son ancienne position
        plateauModifie.erase(anciennePosition);

        // Ajouter l'insecte à sa nouvelle position
        plateauModifie[nouvellePosition] = insecte;
    }

    return plateauModifie;
}



int JoueurIANiveau3::minimax(std::map<Hexagon, Insecte*> plateau, int profondeur, bool maximisateur, int alpha, int beta,
                             std::stack<std::vector<Insecte*>>& pileDeckMax, std::stack<std::vector<Insecte*>>& pileDeckMin) {

    // Récupérer les decks courants depuis la pile
    auto& deckMaxCourant = pileDeckMax.top();
    auto& deckMinCourant = pileDeckMin.top();

    // Cas de base : profondeur zéro ou état terminal
    if (profondeur <= 0 || estTerminal(plateau, deckMaxCourant, deckMinCourant)) {
        return evaluerPlateau(plateau, deckMaxCourant, deckMinCourant);
    }

    if (maximisateur) {
        int meilleurScore = -std::numeric_limits<int>::max();

        // Générer les coups possibles pour le maximisateur
        auto coupsPossibles = genererCoups(plateau, true, deckMaxCourant, deckMinCourant);

        if (coupsPossibles.empty()) {
            // Aucun coup possible, retourner un score d'état terminal
            return evaluerPlateau(plateau, deckMaxCourant, deckMinCourant);
        }

        for (auto& [insecte, positions] : coupsPossibles) {
            for (auto& position : positions) {
                auto deckMaxCopy = deckMaxCourant;
                auto deckMinCopy = deckMinCourant;

                // Simuler le coup
                auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position),
                                                 deckMaxCopy, deckMinCopy, true);

                mettreAJourCoordonneesInsectes(plateauSimule);
                // Empiler les decks simulés
                pileDeckMax.push(deckMaxCopy);
                pileDeckMin.push(deckMinCopy);

                // Appel récursif pour minimisateur
                int score = minimax(plateauSimule, profondeur - 1, false, alpha, beta, pileDeckMax, pileDeckMin);

                // Dépiler les decks après l'appel
                pileDeckMax.pop();
                pileDeckMin.pop();

                // Mise à jour des scores
                meilleurScore = std::max(meilleurScore, score);
                alpha = std::max(alpha, score);

                // Coupure alpha-beta
                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;
        }

        return meilleurScore;

    } else {
        int meilleurScore = std::numeric_limits<int>::max();

        // Générer les coups possibles pour le minimisateur
        auto coupsPossibles = genererCoups(plateau, false, deckMaxCourant, deckMinCourant);

        if (coupsPossibles.empty()) {
            // Aucun coup possible, retourner un score d'état terminal
            return evaluerPlateau(plateau, deckMaxCourant, deckMinCourant);
        }

        for (auto& [insecte, positions] : coupsPossibles) {
            for (auto& position : positions) {
                auto deckMaxCopy = deckMaxCourant;
                auto deckMinCopy = deckMinCourant;

                // Simuler le coup
                auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position),
                                                 deckMaxCopy, deckMinCopy, false);

                mettreAJourCoordonneesInsectes(plateauSimule);
                // Empiler les decks simulés
                pileDeckMax.push(deckMaxCopy);
                pileDeckMin.push(deckMinCopy);

                // Appel récursif pour maximisateur
                int score = minimax(plateauSimule, profondeur - 1, true, alpha, beta, pileDeckMax, pileDeckMin);

                // Dépiler les decks après l'appel
                pileDeckMax.pop();
                pileDeckMin.pop();

                // Mise à jour des scores
                meilleurScore = std::min(meilleurScore, score);
                beta = std::min(beta, score);

                // Coupure alpha-beta
                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;
        }

        return meilleurScore;
    }
}


int JoueurIANiveau3::evaluerPlateau(const std::map<Hexagon, Insecte*>& plateau,
                                    const std::vector<Insecte*>& deckMaximisateur,
                                    const std::vector<Insecte*>& deckMinimisateur) {
    // Sauvegarde des états actuels
    auto sauvegardeCandidats = candidats;
    auto sauvegardeNouveauxCandidats = nouveauxCandidats;

    // Réinitialisation des attributs pour éviter tout conflit
    reinitialiserAttributs();

    // Évaluation des placements
    choisirHeuristiquePourPlacer(this, deckMaximisateur, plateau);
    double scoreMoyenPlacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenPlacement = calculerScoreMoyenDePlacement(this, candidats, plateau);
    }

    // Réinitialisation et évaluation des déplacements
    reinitialiserAttributs();
    choisirHeuristiquePourDeplacer(this, plateau);
    double scoreMoyenDeplacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenDeplacement = calculerScoreMoyenDeDeplacement(this, candidats, plateau);
    }

    // Restauration de l'état initial
    candidats = sauvegardeCandidats;
    nouveauxCandidats = sauvegardeNouveauxCandidats;
    reinitialiserAttributs();

    // Retourner la moyenne pondérée des scores
    return static_cast<int>((scoreMoyenPlacement + scoreMoyenDeplacement) / 2.0);
}


bool JoueurIANiveau3::estTerminal(const std::map<Hexagon, Insecte*>& plateau,
                                  const std::vector<Insecte*>& deckMaximisateur,
                                  const std::vector<Insecte*>& deckMinimisateur) {
    // Vérification de l'état de la reine alliée
    Insecte* reineAlliee = getQueenOnPlateau(plateau);
    if (reineAlliee && estEncerclee(reineAlliee, plateau)) {
        return true; // Défaite
    }

    // Vérification de l'état de la reine adverse
    Insecte* reineAdverse = getReineAdverse(plateau);
    if (reineAdverse && estEncerclee(reineAdverse, plateau)) {
        return true; // Victoire
    }

    // Vérification des coups possibles pour les deux joueurs
    bool coupsMaximisateur = !genererCoups(plateau, true, deckMaximisateur, deckMinimisateur).empty();
    bool coupsMinimisateur = !genererCoups(plateau, false, deckMaximisateur, deckMinimisateur).empty();

    // Égalité si aucun coup n'est possible
    return !coupsMaximisateur && !coupsMinimisateur;
}


std::map<Insecte*, std::vector<Hexagon>> JoueurIANiveau3::genererCoups(
        const std::map<Hexagon, Insecte*>& plateau, bool estMaximisant,
        const std::vector<Insecte*>& deckMaximisateur, const std::vector<Insecte*>& deckMinimisateur) {

    reinitialiserAttributs();
    std::map<Insecte*, std::vector<Hexagon>> coupsPossibles;

    if (estMaximisant) {
        // Générer les placements possibles pour le maximisateur
        choisirHeuristiquePourPlacer(this, deckMaximisateur, plateau);
        std::map<Insecte*, std::vector<Hexagon>> placements = getCandidats();

        reinitialiserAttributs();

        // Générer les déplacements possibles pour le maximisateur
        choisirHeuristiquePourDeplacer(this, plateau);
        std::map<Insecte*, std::vector<Hexagon>> deplacements = getCandidats();

        // Fusion des coups possibles
        coupsPossibles = placements;
        for (const auto& [insecte, positions] : deplacements) {
            coupsPossibles[insecte].insert(coupsPossibles[insecte].end(), positions.begin(), positions.end());
        }
    } else {
        // Générer les coups pour l'adversaire
        coupsPossibles = genererCoupsAdversaire(plateau, deckMaximisateur, deckMinimisateur);
    }

    return coupsPossibles;
}



std::map<Insecte*, std::vector<Hexagon>> JoueurIANiveau3::genererCoupsAdversaire(
        const std::map<Hexagon, Insecte*>& plateau,
        const std::vector<Insecte*>& deckMaximisateur, const std::vector<Insecte*>& deckMinimisateur) {

    reinitialiserAttributs();
    std::map<Insecte*, std::vector<Hexagon>> coupsAdversaire;

    // Générer les placements possibles pour l'adversaire
    choisirHeuristiquePourPlacer(adversaire, deckMinimisateur, plateau);
    std::map<Insecte*, std::vector<Hexagon>> placementsAdversaire = getCandidats();

    reinitialiserAttributs();

    // Générer les déplacements possibles pour l'adversaire
    choisirHeuristiquePourDeplacer(adversaire, plateau);
    std::map<Insecte*, std::vector<Hexagon>> deplacementsAdversaire = getCandidats();

    // Fusion des coups possibles
    coupsAdversaire = placementsAdversaire;
    for (const auto& [insecte, positions] : deplacementsAdversaire) {
        coupsAdversaire[insecte].insert(coupsAdversaire[insecte].end(), positions.begin(), positions.end());
    }

    return coupsAdversaire;
}


bool JoueurIANiveau3::estEncerclee(Insecte* reine, const std::map<Hexagon, Insecte*>& plateau) const {
    // Obtenir la position de la reine à partir du plateau simulé
    Hexagon positionReine = getInsectPosition(plateau, reine);

    // Obtenir les voisins de la position de la reine
    std::vector<Hexagon> voisins = positionReine.getVoisins();

    // Vérifier si tous les voisins sont occupés
    for (const Hexagon& voisin : voisins) {
        auto it = plateau.find(voisin);
        if (it == plateau.end() || it->second == nullptr) {
            return false; // Au moins un voisin est libre
        }
    }
    return true; // Tous les voisins sont occupés
}


std::vector<Insecte*> JoueurIANiveau3::copierDeck(const Joueur* joueur) {
    std::vector<Insecte*> copie;
    for (Insecte* insecte : joueur->getDeck()) {
        copie.push_back(insecte);
    }
    return copie;
}

bool JoueurIANiveau3::estPlacement(const std::map<Hexagon, Insecte*>& plateau, Insecte* insecte) {
    // Vérifier si l'insecte se trouve déjà sur le plateau
    for (const auto& [position, insectePlateau] : plateau) {
        if (insectePlateau == insecte) {
            return false; // L'insecte est déjà placé
        }
    }
    return true; // L'insecte n'est pas encore placé
}

void JoueurIANiveau3::mettreAJourCoordonneesInsectes(const std::map<Hexagon, Insecte*>& plateau) {
    for (auto& [position, insecte] : plateau) {
        if (insecte) {
            insecte->setCoords(position); // Mettre à jour les coordonnées de l'insecte
        }
    }
}















