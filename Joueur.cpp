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

void JoueurIANiveau2::remplirCandidatsAvecDeck(const Joueur* joueur) {
    candidats.clear(); // Réinitialiser les candidats

    // Utiliser un ensemble pour éviter les doublons basés sur le nom des insectes
    std::set<std::string> insectesTraites;

    for (Insecte* insecte : joueur->getDeck()) {
        // Vérifier si l'insecte (par son nom) a déjà été traité
        if (insectesTraites.count(insecte->getNom()) > 0) {
            continue; // Insecte déjà traité
        }
        insectesTraites.insert(insecte->getNom());

        // Ajouter l'insecte et ses placements possibles
        std::vector<Hexagon> placements = insecte->getPlacementsPossibles(this->getPlateau());
        if (!placements.empty()) {
            candidats[insecte] = std::move(placements);
        }
    }
}

void JoueurIANiveau2::remplirCandidatsAvecDeck(const Joueur* joueur, const std::vector<Insecte*>& deckCopie) {
    candidats.clear();
    std::set<std::string> insectesTraites;

    for (Insecte* insecte : deckCopie) {
        if (insectesTraites.count(insecte->getNom()) > 0) {
            continue;
        }
        insectesTraites.insert(insecte->getNom());
        std::vector<Hexagon> placements = insecte->getPlacementsPossibles(this->getPlateau());
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

int JoueurIANiveau2::evaluerCohesion(const Joueur* joueur, const Hexagon& emplacement) const {
    std::vector<Hexagon> voisins = getVoisins(emplacement);
    int score = 0;

    // Ici, le "joueur" est utilisé pour distinguer le propriétaire
    for (const Hexagon& voisin : voisins) {
        auto it = plateau->find(voisin);
        if (it != plateau->end() && it->second && it->second->getOwner() == joueur) {
            score++;
        }
    }

    return score;
}

int JoueurIANiveau2::evaluerPlacementAction(const Joueur* joueur, Insecte* insecte, const Hexagon& emplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    std::map<Hexagon, Insecte*> plateauSimule = plateau;
    plateauSimule[emplacement] = insecte;

    int score = evaluerCohesion(joueur, emplacement);

    std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateauSimule);

    int meilleurScoreDeplacement = std::numeric_limits<int>::min();
    for (const Hexagon& dep : deplacementsPossibles) {
        int scoreDeplacement = evaluerDeplacementAction(joueur, insecte, dep, plateauSimule);
        if (scoreDeplacement > meilleurScoreDeplacement) {
            meilleurScoreDeplacement = scoreDeplacement;
        }
    }

    if (!deplacementsPossibles.empty() && meilleurScoreDeplacement != std::numeric_limits<int>::min()) {
        int potentielFutur = meilleurScoreDeplacement + static_cast<int>(deplacementsPossibles.size());
        score += static_cast<int>(0.75f * potentielFutur);
    }

    return static_cast<int>(0.75f * score);
}

int JoueurIANiveau2::evaluerAttaqueReineAdverse(const Joueur* joueur, Insecte* insecte, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    int score = 0;
    Insecte* reineAdverse = joueur->getReineAdverse(plateau);

    if (reineAdverse) {
        std::vector<Hexagon> voisinsReineAdverse = getVoisins(reineAdverse->getCoords());
        Hexagon positionActuelle = insecte->getCoords();

        bool dejaMenacant = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), positionActuelle) != voisinsReineAdverse.end());
        bool menacantApres = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), nouvelEmplacement) != voisinsReineAdverse.end());

        if (menacantApres && !dejaMenacant) {
            score += 10;
        }
        if (dejaMenacant && !menacantApres) {
            score -= 5;
        }
    }

    return score;
}

int JoueurIANiveau2::evaluerProtectionReine(const Joueur* joueur, Insecte* insecte, const Hexagon& ancienneEmplacement, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    int score = 0;
    Insecte* reineAlliee = joueur->getQueenOnPlateau(plateau);
    if (!reineAlliee) return score;

    std::vector<Hexagon> voisinsReineAvant = getVoisins(reineAlliee->getCoords());
    std::vector<Hexagon> ennemisAvant = reineAlliee->getVoisinsEnnemis(voisinsReineAvant, plateau);
    size_t nbEnnemisAvant = ennemisAvant.size();

    std::map<Hexagon, Insecte*> plateauSimule = plateau;
    plateauSimule.erase(ancienneEmplacement);
    plateauSimule[nouvelEmplacement] = insecte;

    Insecte* reineAllieeSimulee = joueur->getQueenOnPlateau(plateauSimule);
    if (!reineAllieeSimulee) return score;

    std::vector<Hexagon> voisinsReineApres = getVoisins(reineAllieeSimulee->getCoords());
    std::vector<Hexagon> ennemisApres = reineAllieeSimulee->getVoisinsEnnemis(voisinsReineApres, plateauSimule);
    size_t nbEnnemisApres = ennemisApres.size();

    int difference = (int)nbEnnemisAvant - (int)nbEnnemisApres;

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

int JoueurIANiveau2::evaluerBlocageInsecteImportant(const Joueur* joueur, Insecte* insecte, const Hexagon& anciennePos, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    int bonusBlocage = 7;
    std::vector<Hexagon> voisins = nouvelEmplacement.getVoisins();

    std::vector<Insecte*> insectesImportants;
    for (const Hexagon& v : voisins) {
        auto it = plateau.find(v);
        if (it != plateau.end() && it->second != nullptr) {
            Insecte* adv = it->second;
            if (adv->getOwner() != joueur && (adv->isQueen() || adv->getNom() == "Fourmi")) {
                std::vector<Hexagon> deplacementsAvant = adv->deplacementsPossibles(plateau);
                if (!deplacementsAvant.empty()) {
                    insectesImportants.push_back(adv);
                }
            }
        }
    }

    if (insectesImportants.empty()) {
        return 0;
    }

    std::map<Hexagon, Insecte*> plateauSimule = plateau;
    plateauSimule.erase(anciennePos);
    plateauSimule[nouvelEmplacement] = insecte;

    int totalBonus = 0;
    for (Insecte* adv : insectesImportants) {
        std::vector<Hexagon> deplacementsApres = adv->deplacementsPossibles(plateauSimule);
        if (deplacementsApres.empty()) {
            totalBonus += bonusBlocage;
        }
    }

    return totalBonus;
}

int JoueurIANiveau2::evaluerDeplacementAction(const Joueur* joueur, Insecte* insecte, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    Hexagon anciennePos = insecte->getCoords();

    int scoreTotal = 0;
    scoreTotal += evaluerAttaqueReineAdverse(joueur, insecte, nouvelEmplacement, plateau);
    scoreTotal += evaluerProtectionReine(joueur, insecte, anciennePos, nouvelEmplacement, plateau);
    scoreTotal += evaluerBlocageInsecteImportant(joueur, insecte, anciennePos, nouvelEmplacement, plateau);

    return scoreTotal;
}


std::vector<std::pair<Hexagon,int>> JoueurIANiveau2::evaluerEtTrierMouvements(const Joueur* joueur, Insecte* insecte, const std::vector<Hexagon>& options, bool estPlacement) const {
    std::vector<std::pair<Hexagon,int>> mouvementsAvecScore;
    for (const Hexagon& option : options) {
        int score = estPlacement ?
                    evaluerPlacementAction(joueur, insecte, option, getPlateau()) :
                    evaluerDeplacementAction(joueur, insecte, option, getPlateau());
        mouvementsAvecScore.emplace_back(option, score);
    }

    std::sort(mouvementsAvecScore.begin(), mouvementsAvecScore.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    return mouvementsAvecScore;
}

std::vector<Hexagon> JoueurIANiveau2::extraireMeilleursMouvements(const std::vector<std::pair<Hexagon,int>>& mouvementsTries, int nombreMax) const {
    std::vector<Hexagon> meilleurs;
    for (int i = 0; i < std::min((int)mouvementsTries.size(), nombreMax); ++i) {
        meilleurs.push_back(mouvementsTries[i].first);
    }
    return meilleurs;
}

int JoueurIANiveau2::calculerScoreMaxParInsecte(const Joueur* joueur, Insecte* insecte, bool estPlacement) const {
    auto it = nouveauxCandidats.find(insecte);
    if (it == nouveauxCandidats.end() || it->second.empty()) {
        return std::numeric_limits<int>::min();
    }

    int meilleurScore = std::numeric_limits<int>::min();
    for (const Hexagon& option : it->second) {
        int score = estPlacement ?
                    evaluerPlacementAction(joueur, insecte, option, getPlateau()) :
                    evaluerDeplacementAction(joueur, insecte, option, getPlateau());
        if (score > meilleurScore) {
            meilleurScore = score;
        }
    }

    return meilleurScore;
}

void JoueurIANiveau2::filtrerMeilleursInsectes(const Joueur* joueur, std::map<Insecte*, std::vector<Hexagon>>& insectesCandidats, int nombreMeilleursInsectes, bool estPlacement) {
    if (insectesCandidats.empty()) return;

    std::vector<std::pair<Insecte*, int>> insectesAvecScore;
    for (const auto& [insecte, _] : insectesCandidats) {
        int scoreMax = calculerScoreMaxParInsecte(joueur, insecte, estPlacement);
        insectesAvecScore.emplace_back(insecte, scoreMax);
    }

    std::sort(insectesAvecScore.begin(), insectesAvecScore.end(),
              [](const auto& a, const auto& b){
                  return a.second > b.second;
              });

    std::map<Insecte*, std::vector<Hexagon>> meilleursInsectes;
    for (int i = 0; i < std::min((int)insectesAvecScore.size(), nombreMeilleursInsectes); ++i) {
        Insecte* insecte = insectesAvecScore[i].first;
        meilleursInsectes[insecte] = insectesCandidats[insecte];
    }

    insectesCandidats = std::move(meilleursInsectes);
}


void JoueurIANiveau2::remplirCandidatsAvecPlateau(const Joueur* joueur) {
    candidats.clear();

    for (const auto& [position, insecte] : getPlateau()) {
        if (insecte->getOwner() == joueur) {
            std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(getPlateau());
            if (!deplacementsPossibles.empty()) {
                candidats[insecte] = std::move(deplacementsPossibles);
            }
        }
    }
}

void JoueurIANiveau2::choisirHeuristiquePourDeplacer(const Joueur* joueur) {
    remplirCandidatsAvecPlateau(joueur);

    int nombreMaxDeplacements = 1;
    nouveauxCandidats.clear();

    for (const auto& [insecte, deplacements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(joueur, insecte, deplacements, false);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxDeplacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    filtrerMeilleursInsectes(joueur, nouveauxCandidats, 2, false);

    candidats = nouveauxCandidats;

    if (!candidats.empty()) {
        actionChoisie = DEPLACER;
    }
}


void JoueurIANiveau2::choisirHeuristiquePourPlacer(const Joueur* joueur) {

    remplirCandidatsAvecDeck(joueur);

    int nombreMaxPlacements = 1;
    nouveauxCandidats.clear();

    for (const auto& [insecte, placements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(joueur, insecte, placements, true);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxPlacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    filtrerMeilleursInsectes(joueur, nouveauxCandidats, 2, true);

    candidats = nouveauxCandidats;
    if (!candidats.empty()) {
        actionChoisie = PLACER;
    }
}

void JoueurIANiveau2::choisirHeuristiquePourPlacer(const Joueur* joueur, std::vector<Insecte*> deckCopie) {
    std::cout << "Début de choisirHeuristiquePourPlacer" << std::endl;

    remplirCandidatsAvecDeck(joueur, deckCopie);
    std::cout << "Remplir candidats terminé" << std::endl;

    int nombreMaxPlacements = 1;
    nouveauxCandidats.clear();
    std::cout << "Nouveaux candidats réinitialisés" << std::endl;

    for (const auto& [insecte, placements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(joueur, insecte, placements, true);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxPlacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
        std::cout << "Insecte traité : " << insecte->getNom() << std::endl;
    }

    filtrerMeilleursInsectes(joueur, nouveauxCandidats, 2, true);
    std::cout << "Meilleurs insectes filtrés" << std::endl;

    candidats = nouveauxCandidats;
    if (!candidats.empty()) {
        actionChoisie = PLACER;
        std::cout << "Action choisie : PLACER" << std::endl;
    }
    std::cout << "Fin de choisirHeuristiquePourPlacer" << std::endl;
}


double JoueurIANiveau2::calculerScoreMoyenDePlacement(const Joueur* joueur, const std::map<Insecte*, std::vector<Hexagon>>& candidats) const {
    if (candidats.empty()) {
        return 0.0;
    }

    double somme = 0.0;
    int count = 0;

    for (const auto& [insecte, emplacements] : candidats) {
        for (const Hexagon& emplacement : emplacements) {
            int score = evaluerPlacementAction(joueur, insecte, emplacement, getPlateau());
            somme += score;
            ++count;
        }
    }

    return (count > 0) ? (somme / count) : 0.0;
}

double JoueurIANiveau2::calculerScoreMoyenDeDeplacement(const Joueur* joueur, const std::map<Insecte*, std::vector<Hexagon>>& candidats) const {
    if (candidats.empty()) {
        return 0.0;
    }

    double somme = 0.0;
    int count = 0;

    for (const auto& [insecte, deplacements] : candidats) {
        for (const Hexagon& deplacement : deplacements) {
            int score = evaluerDeplacementAction(joueur, insecte, deplacement, getPlateau());
            somme += score;
            ++count;
        }
    }

    return (count > 0) ? (somme / count) : 0.0;
}


int JoueurIANiveau2::getInputForAction() {
    reinitialiserAttributs();

    choisirHeuristiquePourPlacer(this);
    double scoreMoyenPlacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenPlacement = calculerScoreMoyenDePlacement(this, candidats);
    }
    std::map<Insecte*, std::vector<Hexagon>> candidatsPlacement = candidats;

    reinitialiserAttributs();
    choisirHeuristiquePourDeplacer(this);
    double scoreMoyenDeplacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenDeplacement = calculerScoreMoyenDeDeplacement(this, candidats);
    }

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

std::map<Hexagon, Insecte*> JoueurIANiveau3::simulerCoup(const std::map<Hexagon, Insecte*>& plateau,
                                        const std::pair<Insecte*, Hexagon>& coup,
                                        std::vector<Insecte*>& deckMaximisateur,
                                        std::vector<Insecte*>& deckMinimisateur,
                                        bool estMaximisant) {
    std::map<Hexagon, Insecte*> plateauSimule = plateau;
    Insecte* insecte = coup.first;
    const Hexagon& nouvellePosition = coup.second;

    bool estUnPlacement = estPlacement(plateauSimule, insecte);

    if (estUnPlacement) {
        plateauSimule[nouvellePosition] = insecte;

        std::vector<Insecte*>& deckActuel = estMaximisant ? deckMaximisateur : deckMinimisateur;
        auto it = std::find(deckActuel.begin(), deckActuel.end(), insecte);

        // Vérification si l'insecte est introuvable dans le deck
        if (it != deckActuel.end()) {
            deckActuel.erase(it);
        } else {
            throw std::runtime_error("Insecte non trouvé dans le deck pour le placement.");
        }
    } else {
        Hexagon anciennePosition;
        bool trouve = false;

        // Rechercher la position actuelle de l'insecte sur le plateau
        for (const auto& [pos, insectePlateau] : plateauSimule) {
            if (insectePlateau == insecte) {
                anciennePosition = pos;
                trouve = true;
                break;
            }
        }

        if (trouve) {
            plateauSimule.erase(anciennePosition);
            plateauSimule[nouvellePosition] = insecte;
        } else {
            throw std::runtime_error("Insecte non trouvé sur le plateau pour le déplacement.");
        }
    }


    return plateauSimule;
}

int JoueurIANiveau3::minimax(std::map<Hexagon, Insecte*> plateau, int profondeur, bool maximisateur, int alpha, int beta,
            std::stack<std::vector<Insecte*>>& pileDeckMax, std::stack<std::vector<Insecte*>>& pileDeckMin) {

    auto& deckMaxCourant = pileDeckMax.top();
    auto& deckMinCourant = pileDeckMin.top();

    // Cas de base
    if (profondeur <= 0 || estTerminal(plateau, deckMaxCourant, deckMinCourant)) {
        return evaluerPlateau(plateau, deckMaxCourant, deckMinCourant);
    }

    if (maximisateur) {
        int meilleurScore = -std::numeric_limits<int>::max();
        // Générer les coups possibles pour le maximisateur
        auto coupsPossibles = genererCoups(plateau, true, deckMaxCourant, deckMinCourant);

        for (auto& [insecte, positions] : coupsPossibles) {
            for (auto& position : positions) {
                auto deckMaxCopy = deckMaxCourant;
                auto deckMinCopy = deckMinCourant;

                // Simuler le coup
                auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position),
                                                 deckMaxCopy, deckMinCopy, true);

                // Empiler avant l'appel récursif
                pileDeckMax.push(deckMaxCopy);
                pileDeckMin.push(deckMinCopy);

                int score = minimax(plateauSimule, profondeur - 1, false, alpha, beta, pileDeckMax, pileDeckMin);

                // Dépiler
                pileDeckMax.pop();
                pileDeckMin.pop();

                meilleurScore = std::max(meilleurScore, score);
                alpha = std::max(alpha, score);
                if (beta <= alpha) break; // Coupure beta
            }
            if (beta <= alpha) break;
        }
        return meilleurScore;
    } else {
        int meilleurScore = std::numeric_limits<int>::max();
        // Générer les coups possibles pour le minimisateur
        auto coupsPossibles = genererCoups(plateau, false, deckMaxCourant, deckMinCourant);

        for (auto& [insecte, positions] : coupsPossibles) {
            for (auto& position : positions) {
                auto deckMaxCopy = deckMaxCourant;
                auto deckMinCopy = deckMinCourant;

                // Simuler le coup
                auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position),
                                                 deckMaxCopy, deckMinCopy, false);

                // Empiler avant l'appel récursif
                pileDeckMax.push(deckMaxCopy);
                pileDeckMin.push(deckMinCopy);

                int score = minimax(plateauSimule, profondeur - 1, true, alpha, beta, pileDeckMax, pileDeckMin);

                // Dépiler
                pileDeckMax.pop();
                pileDeckMin.pop();

                meilleurScore = std::min(meilleurScore, score);
                beta = std::min(beta, score);
                if (beta <= alpha) break; // Coupure alpha
            }
            if (beta <= alpha) break;
        }
        return meilleurScore;
    }
}

int JoueurIANiveau3::evaluerPlateau(const std::map<Hexagon, Insecte*>& plateau,
                   const std::vector<Insecte*>& deckMaximisateur,
                   const std::vector<Insecte*>& deckMinimisateur) {
    // Évaluation inchangée, utilise deckMaximisateur/Minimisateur passés en param
    auto sauvegardeCandidats = candidats;
    auto sauvegardeNouveauxCandidats = nouveauxCandidats;
    const std::map<Hexagon, Insecte*>* plateauInitial = this->plateau;

    reinitialiserAttributs();
    choisirHeuristiquePourPlacer(this);
    double scoreMoyenPlacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenPlacement = calculerScoreMoyenDePlacement(this, candidats);
    }

    reinitialiserAttributs();
    choisirHeuristiquePourDeplacer(this);
    double scoreMoyenDeplacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenDeplacement = calculerScoreMoyenDeDeplacement(this, candidats);
    }

    this->plateau = plateauInitial;
    candidats = sauvegardeCandidats;
    nouveauxCandidats = sauvegardeNouveauxCandidats;
    reinitialiserAttributs();

    return static_cast<int>((scoreMoyenPlacement + scoreMoyenDeplacement) / 2.0);
}

bool JoueurIANiveau3::estTerminal(const std::map<Hexagon, Insecte*>& plateau,
                 const std::vector<Insecte*>& deckMaximisateur,
                 const std::vector<Insecte*>& deckMinimisateur) {
    Insecte* reineAlliee = getQueenOnPlateau(plateau);
    Insecte* reineAdverse = getReineAdverse(plateau);

    if (reineAlliee && estEncerclee(reineAlliee, plateau)) {
        return true; // Défaite
    }

    if (reineAdverse && estEncerclee(reineAdverse, plateau)) {
        return true; // Victoire
    }

    bool coupsMaximisateur = !genererCoups(plateau, true, deckMaximisateur, deckMinimisateur).empty();
    bool coupsMinimisateur = !genererCoups(plateau, false, deckMaximisateur, deckMinimisateur).empty();

    if (!coupsMaximisateur && !coupsMinimisateur) {
        return true; // Égalité
    }

    return false;
}


bool JoueurIANiveau3::estEncerclee(Insecte* reine, const std::map<Hexagon, Insecte*>& plateau) const {
    std::vector<Hexagon> voisins = reine->getCoords().getVoisins();
    for (const Hexagon& voisin : voisins) {
        auto it = plateau.find(voisin);
        if (it == plateau.end() || it->second == nullptr) {
            return false; // Au moins un voisin est libre
        }
    }
    return true;
}

std::vector<Insecte*> JoueurIANiveau3::copierDeck(const Joueur* joueur) {
    std::vector<Insecte*> copie;
    for (Insecte* insecte : joueur->getDeck()) {
        copie.push_back(insecte);
    }
    return copie;
}


std::map<Insecte*, std::vector<Hexagon>> JoueurIANiveau3::genererCoups(
        const std::map<Hexagon, Insecte*>& plateau, bool estMaximisant,
        const std::vector<Insecte*>& deckMaximisateur, const std::vector<Insecte*>& deckMinimisateur) {

    reinitialiserAttributs();

    std::map<Insecte*, std::vector<Hexagon>> coupsPossibles;

    if (estMaximisant) {
        // On utilise la surcharge avec deckMaximisateur
        choisirHeuristiquePourPlacer(this, deckMaximisateur);
        std::map<Insecte*, std::vector<Hexagon>> placements = getCandidats();

        reinitialiserAttributs();
        choisirHeuristiquePourDeplacer(this);
        std::map<Insecte*, std::vector<Hexagon>> deplacements = getCandidats();

        coupsPossibles = placements;
        for (const auto& [insecte, positions] : deplacements) {
            coupsPossibles[insecte].insert(coupsPossibles[insecte].end(), positions.begin(), positions.end());
        }
    } else {
        coupsPossibles = genererCoupsAdversaire(plateau, deckMaximisateur, deckMinimisateur);
    }

    return coupsPossibles;
}


std::map<Insecte*, std::vector<Hexagon>> JoueurIANiveau3::genererCoupsAdversaire(
        const std::map<Hexagon, Insecte*>& plateau,
        const std::vector<Insecte*>& deckMaximisateur, const std::vector<Insecte*>& deckMinimisateur) {

    std::cout << "=== Début de genererCoupsAdversaire ===" << std::endl;

    // Afficher les decks avant de commencer
    std::cout << "Deck de l'adversaire avant traitement : " << std::endl;
    for (const auto& insecte : deckMinimisateur) {
        std::cout << " - " << insecte->getNom() << std::endl;
    }

    reinitialiserAttributs();
    std::cout << "Attributs réinitialisés." << std::endl;

    choisirHeuristiquePourPlacer(adversaire, deckMinimisateur);
    std::map<Insecte*, std::vector<Hexagon>> placementsAdversaire = getCandidats();

    std::cout << "Placements possibles pour l'adversaire : " << std::endl;
    for (const auto& [insecte, positions] : placementsAdversaire) {
        std::cout << "Insecte : " << insecte->getNom() << " -> Positions : ";
        for (const auto& pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }

    reinitialiserAttributs();
    std::cout << "Attributs réinitialisés à nouveau pour les déplacements." << std::endl;

    choisirHeuristiquePourDeplacer(adversaire);
    std::map<Insecte*, std::vector<Hexagon>> deplacementsAdversaire = getCandidats();

    std::cout << "Déplacements possibles pour l'adversaire : " << std::endl;
    for (const auto& [insecte, positions] : deplacementsAdversaire) {
        std::cout << "Insecte : " << insecte->getNom() << " -> Positions : ";
        for (const auto& pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }

    std::map<Insecte*, std::vector<Hexagon>> coupsAdversaire = placementsAdversaire;
    for (const auto& [insecte, positions] : deplacementsAdversaire) {
        coupsAdversaire[insecte].insert(coupsAdversaire[insecte].end(), positions.begin(), positions.end());
    }

    std::cout << "Coups combinés (placements + déplacements) pour l'adversaire : " << std::endl;
    for (const auto& [insecte, positions] : coupsAdversaire) {
        std::cout << "Insecte : " << insecte->getNom() << " -> Positions : ";
        for (const auto& pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "=== Fin de genererCoupsAdversaire ===" << std::endl;

    return coupsAdversaire;
}

bool JoueurIANiveau3::estPlacement(const std::map<Hexagon, Insecte*>& plateau, Insecte* insecte) {
    for (const auto& [position, insectePlateau] : plateau) {
        if (insectePlateau == insecte) {
            return false; // L'insecte est déjà placé
        }
    }
    return true; // L'insecte n'est pas encore placé
}












