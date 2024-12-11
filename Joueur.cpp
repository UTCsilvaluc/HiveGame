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

void JoueurIANiveau2::remplirCandidatsAvecDeck() {
    candidats.clear(); // Réinitialiser les candidats

    // Ajouter chaque insecte du deck à candidats avec les mêmes placements possibles
    for (Insecte* insecte : getDeck()) {
        candidats[insecte] = insecte->getPlacementsPossibles(getPlateau());
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

int JoueurIANiveau2::evaluerCohesion(const Hexagon& emplacement) const {
    std::vector<Hexagon> voisins = getVoisins(emplacement);
    int score = 0;

    for (const Hexagon& voisin : voisins) {
        auto it = plateau->find(voisin);
        if (it != plateau->end() && it->second && it->second->getOwner() == this) {
            score++;
        }
    }

    return score;
}


int JoueurIANiveau2::evaluerPlacementAction(Insecte* insecte, const Hexagon& emplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    // Simuler le placement
    std::map<Hexagon, Insecte*> plateauSimule = plateau;
    plateauSimule[emplacement] = insecte;

    // Critère 1 : Cohésion avec la ruche
    int score = evaluerCohesion(emplacement);

    // Récupération des déplacements possibles après placement
    std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateauSimule);

    // Évaluer la qualité potentielle de ces déplacements futurs
    int meilleurScoreDeplacement = std::numeric_limits<int>::min();

    for (const Hexagon& dep : deplacementsPossibles) {
        int scoreDeplacement = evaluerDeplacementAction(insecte, dep, plateauSimule);
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

int JoueurIANiveau2::evaluerAttaqueReineAdverse(Insecte* insecte, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    int score = 0;
    Insecte* reineAdverse = getReineAdverse(plateau);

    if (reineAdverse) {
        std::vector<Hexagon> voisinsReineAdverse = getVoisins(reineAdverse->getCoords());

        // Position actuelle de l’insecte
        Hexagon positionActuelle = insecte->getCoords();

        bool dejaMenacant = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), positionActuelle) != voisinsReineAdverse.end());
        bool menacantApres = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), nouvelEmplacement) != voisinsReineAdverse.end());

        // Si l'insecte n'était pas menaçant avant et le devient, on ajoute un bonus
        if (menacantApres && !dejaMenacant) {
            score += 10;
        }

        // Si l'insecte était menaçant avant et qu'il ne l'est plus, on ajoute un malus
        if (dejaMenacant && !menacantApres) {
            score -= 5; // Par exemple un malus de 5
        }
    }

    return score;
}



int JoueurIANiveau2::evaluerProtectionReine(Insecte* insecte, const Hexagon& ancienEmplacement, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    int score = 0;
    Insecte* reineAlliee = getQueenOnPlateau(plateau);
    if (!reineAlliee) {
        return score; // Pas de reine sur le plateau, pas de bonus
    }

    std::vector<Hexagon> voisinsReineAvant = getVoisins(reineAlliee->getCoords());
    std::vector<Hexagon> ennemisAvant = reineAlliee->getVoisinsEnnemis(voisinsReineAvant, plateau);
    size_t nbEnnemisAvant = ennemisAvant.size();

    // Simuler le déplacement
    std::map<Hexagon, Insecte*> plateauSimule = plateau;
    plateauSimule.erase(ancienEmplacement);
    plateauSimule[nouvelEmplacement] = insecte;

    Insecte* reineAllieeSimulee = getQueenOnPlateau(plateauSimule);
    if (!reineAllieeSimulee) {
        return score; // La reine n'est plus sur le plateau (étrange, mais on sécurise)
    }

    std::vector<Hexagon> voisinsReineApres = getVoisins(reineAllieeSimulee->getCoords());
    std::vector<Hexagon> ennemisApres = reineAllieeSimulee->getVoisinsEnnemis(voisinsReineApres, plateauSimule);
    size_t nbEnnemisApres = ennemisApres.size();

    int difference = (int)nbEnnemisAvant - (int)nbEnnemisApres;

    // S’il y a une amélioration
    if (difference > 0) {
        // Accentuer le bonus si la situation était critique avant
        float multiplicateur = 1.0f;
        if (nbEnnemisAvant > 4) {
            multiplicateur = 1.5f; // Situation très critique, bonus plus élevé
        }

        // Différencier reine ou allié
        int baseBonus = 0;
        if (insecte == reineAlliee) {
            // La reine se protège elle-même
            // bonus de base 10 par ennemi écarté au minimum
            baseBonus = 10 * difference;
        } else {
            // Un allié aide la reine
            // bonus de base 5 par ennemi écarté
            baseBonus = 5 * difference;
        }

        score += (int)(baseBonus * multiplicateur);
    }
    else if (difference < 0) {
        // La situation empire, ajouter un malus
        // Par exemple -5 par ennemi supplémentaire
        score -= 5 * (-difference);
    }
    // Si difference == 0, pas de changement, pas de bonus ni malus

    return score;
}


int JoueurIANiveau2::evaluerBlocageInsecteImportant(Insecte* insecte, const Hexagon& anciennePos, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    int bonusBlocage = 7;

    // Récupérer les voisins du nouvel emplacement
    std::vector<Hexagon> voisins = nouvelEmplacement.getVoisins();

    // Chercher tous les insectes adverses importants (Reine ou Fourmi)
    std::vector<Insecte*> insectesImportants;
    for (const Hexagon& v : voisins) {
        auto it = plateau.find(v);
        if (it != plateau.end() && it->second != nullptr) {
            Insecte* adv = it->second;
            if (adv->getOwner() != this && (adv->isQueen() || adv->getNom() == "Fourmi")) {
                std::vector<Hexagon> deplacementsAvant = adv->deplacementsPossibles(plateau);
                // Ne garder l'insecte que s'il a des déplacements avant notre coup
                if (!deplacementsAvant.empty()) {
                    insectesImportants.push_back(adv);
                }
            }
        }
    }

    // S'il n'y a aucun insecte important mobile, on arrête ici
    if (insectesImportants.empty()) {
        return 0;
    }

    // Simuler notre déplacement
    std::map<Hexagon, Insecte*> plateauSimule = plateau;
    plateauSimule.erase(anciennePos);
    plateauSimule[nouvelEmplacement] = insecte;

    int totalBonus = 0;

    // Vérifier après notre déplacement
    for (Insecte* adv : insectesImportants) {
        // On sait déjà que adv avait des déplacements avant
        // Vérifier s'il en a maintenant
        std::vector<Hexagon> deplacementsApres = adv->deplacementsPossibles(plateauSimule);
        if (deplacementsApres.empty()) {
            // L'insecte est maintenant bloqué !
            totalBonus += bonusBlocage;
        }
    }

    return totalBonus;
}


int JoueurIANiveau2::evaluerDeplacementAction(Insecte* insecte, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const {
    Hexagon anciennePos = insecte->getCoords();

    int scoreTotal = 0;
    scoreTotal += evaluerAttaqueReineAdverse(insecte, nouvelEmplacement, plateau);
    scoreTotal += evaluerProtectionReine(insecte, anciennePos, nouvelEmplacement, plateau);
    scoreTotal += evaluerBlocageInsecteImportant(insecte, anciennePos, nouvelEmplacement, plateau);

    return scoreTotal;
}





std::vector<std::pair<Hexagon,int>> JoueurIANiveau2::evaluerEtTrierMouvements(Insecte* insecte, const std::vector<Hexagon>& options, bool estPlacement) const {
    std::vector<std::pair<Hexagon,int>> mouvementsAvecScore;
    for (const Hexagon& option : options) {
        int score = estPlacement ?
                    evaluerPlacementAction(insecte, option, getPlateau()) :
                    evaluerDeplacementAction(insecte, option, getPlateau());
        mouvementsAvecScore.emplace_back(option, score);
    }

    // Tri par score décroissant
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

int JoueurIANiveau2::calculerScoreMaxParInsecte(Insecte* insecte, bool estPlacement) const {
    auto it = nouveauxCandidats.find(insecte);
    if (it == nouveauxCandidats.end() || it->second.empty()) {
        return std::numeric_limits<int>::min();
    }

    int meilleurScore = std::numeric_limits<int>::min();
    for (const Hexagon& option : it->second) {
        int score = estPlacement ?
                    evaluerPlacementAction(insecte, option, getPlateau()) :
                    evaluerDeplacementAction(insecte, option, getPlateau());
        if (score > meilleurScore) {
            meilleurScore = score;
        }
    }

    return meilleurScore;
}

void JoueurIANiveau2::filtrerMeilleursInsectes(std::map<Insecte*, std::vector<Hexagon>>& insectesCandidats, int nombreMeilleursInsectes, bool estPlacement) {
    if (insectesCandidats.empty()) return;

    std::vector<std::pair<Insecte*, int>> insectesAvecScore;
    for (const auto& [insecte, _] : insectesCandidats) {
        int scoreMax = calculerScoreMaxParInsecte(insecte, estPlacement);
        insectesAvecScore.emplace_back(insecte, scoreMax);
    }

    std::sort(insectesAvecScore.begin(), insectesAvecScore.end(),
              [](const auto& a, const auto& b){
                  return a.second > b.second;
              });

    // On ne garde que les meilleurs insectes
    std::map<Insecte*, std::vector<Hexagon>> meilleursInsectes;
    for (int i = 0; i < std::min((int)insectesAvecScore.size(), nombreMeilleursInsectes); ++i) {
        Insecte* insecte = insectesAvecScore[i].first;
        meilleursInsectes[insecte] = insectesCandidats[insecte];
    }

    insectesCandidats = std::move(meilleursInsectes);
}


void JoueurIANiveau2::remplirCandidatsAvecPlateau() {
    candidats.clear(); // RÃ©initialiser les candidats

    for (const auto& [position, insecte] : getPlateau()) {
        if (insecte->getOwner() == this) { // VÃ©rifier que l'insecte appartient au joueur
            std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(getPlateau());
            if (!deplacementsPossibles.empty()) {
                candidats[insecte] = std::move(deplacementsPossibles); // Ajouter l'insecte et ses dÃ©placements possibles
            }
        }
    }
}

void JoueurIANiveau2::choisirHeuristiquePourDeplacer() {
    remplirCandidatsAvecPlateau();

    int nombreMaxDeplacements = 1;
    nouveauxCandidats.clear();

    // 1. Pour chaque insecte, évaluer et trier les déplacements
    for (const auto& [insecte, deplacements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(insecte, deplacements, false);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxDeplacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    // 2. Filtrer les meilleurs insectes
    filtrerMeilleursInsectes(nouveauxCandidats, 2, false);

    candidats = nouveauxCandidats;

    // Prioriser les candidats
    if (!candidats.empty()) {
        actionChoisie = DEPLACER;
    }
}


void JoueurIANiveau2::choisirHeuristiquePourPlacer() {
    remplirCandidatsAvecDeck();
    int nombreMaxPlacements = 1;
    nouveauxCandidats.clear();

    // 1. Pour chaque insecte, évaluer et trier les placements
    for (const auto& [insecte, placements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(insecte, placements, true);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxPlacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    // 2. Filtrer les meilleurs insectes
    filtrerMeilleursInsectes(nouveauxCandidats, 2, true);

    candidats = nouveauxCandidats;
    if (!candidats.empty()) {
        actionChoisie = PLACER;
    }
}

double JoueurIANiveau2::calculerScoreMoyenDePlacement(const std::map<Insecte*, std::vector<Hexagon>>& candidats) const {
    if (candidats.empty()) {
        return 0.0;
    }

    double somme = 0.0;
    int count = 0;

    for (const auto& [insecte, emplacements] : candidats) {
        for (const Hexagon& emplacement : emplacements) {
            int score = evaluerPlacementAction(insecte, emplacement, getPlateau());
            somme += score;
            ++count;
        }
    }

    return (count > 0) ? (somme / count) : 0.0;
}

double JoueurIANiveau2::calculerScoreMoyenDeDeplacement(const std::map<Insecte*, std::vector<Hexagon>>& candidats) const {
    if (candidats.empty()) {
        return 0.0;
    }

    double somme = 0.0;
    int count = 0;

    for (const auto& [insecte, deplacements] : candidats) {
        for (const Hexagon& deplacement : deplacements) {
            int score = evaluerDeplacementAction(insecte, deplacement, getPlateau());
            somme += score;
            ++count;
        }
    }

    return (count > 0) ? (somme / count) : 0.0;
}





int JoueurIANiveau2::getInputForAction() {
    reinitialiserAttributs();

    // Évaluer les placements
    choisirHeuristiquePourPlacer(); // Remplit candidats
    double scoreMoyenPlacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenPlacement = calculerScoreMoyenDePlacement(candidats);
    }
    std::map<Insecte*, std::vector<Hexagon>> candidatsPlacement = candidats;

    // Évaluer les déplacements
    reinitialiserAttributs();
    choisirHeuristiquePourDeplacer(); // Remplit candidats
    double scoreMoyenDeplacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenDeplacement = calculerScoreMoyenDeDeplacement(candidats);
    }

    // Comparer les scores moyens
    if (scoreMoyenPlacement > scoreMoyenDeplacement) {
        // Choisir l'action PLACER
        candidats = candidatsPlacement;
        actionChoisie = PLACER;
    } else if (scoreMoyenDeplacement > 0.0) {
        // Choisir l'action DEPLACER
        // candidats correspond aux déplacements car on n'a pas réinitialisé entre temps
        actionChoisie = DEPLACER;
    } else {
        // Si les deux sont nuls, faire un choix aléatoire
        actionChoisie = (randomFloat() < 0.5) ? PLACER : DEPLACER;
    }

    return static_cast<int>(actionChoisie);
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











