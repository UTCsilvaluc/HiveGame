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
    //newDeck.push_back(new Termite(Hexagon(0, 0) , joueur));  // Reine Abeille x1
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

    jsonData << "  \"deck\": [\n";
    for (size_t i = 0; i < deck.size(); ++i) {
        if (i != 0) {
            jsonData << "\n";  // S�parateur entre les insectes
        }
        jsonData << deck[i]->toJson();  // Appel de toJson pour chaque Insecte
    }
    jsonData << "\n]\n";

    jsonData << "}";
    return jsonData.str();
}


// Réécriture complète de remplirCandidatsAvecDeck
void JoueurIANiveau2::remplirCandidatsAvecDeck(const Joueur* joueur,
                                               const std::vector<Insecte*>& deck,
                                               const std::map<Hexagon, Insecte*>& plateauSimule) {
    candidats.clear();

    Insecte* reineAdverse = joueur->getReineAdverse(plateauSimule);
    Insecte* reineAlliee  = joueur->getQueenOnPlateau(plateauSimule);

    std::set<std::string> insectesTraites;
    for (Insecte* insecte : deck) {
        const std::string& nom = insecte->getNom();
        if (insectesTraites.count(nom) > 0) {
            // Insecte de ce type déjà traité
            continue;
        }
        insectesTraites.insert(nom);

        // Obtenir tous les placements possibles
        std::vector<Hexagon> placements = insecte->getPlacementsPossibles(plateauSimule);

        filtrerPlacements(placements, reineAdverse, reineAlliee, plateauSimule);

        // Ajouter aux candidats si des placements valides restent
        if (!placements.empty()) {
            candidats[insecte] = std::move(placements);
        }
    }
}


void JoueurIANiveau2::remplirCandidatsAvecPlateau(const Joueur* joueur,
                                                  const std::map<Hexagon, Insecte*>& plateauSimule) {
    candidats.clear();

    Insecte* reineAdverse = joueur->getReineAdverse(plateauSimule);
    Insecte* reineAlliee  = joueur->getQueenOnPlateau(plateauSimule);
    for (const auto& [position, insecte] : plateauSimule) {
        if (!insecte || insecte->getOwner() != joueur)
            continue;

        std::vector<Hexagon> deplacements = insecte->deplacementsPossibles(plateauSimule);

        filtrerPlacements(deplacements, reineAdverse, reineAlliee, plateauSimule);

        if (!deplacements.empty()) {
            candidats[insecte] = std::move(deplacements);
        }
    }
}

void JoueurIANiveau2::filtrerPlacements(std::vector<Hexagon>& positions,
                                        const Insecte* reineAdverse,
                                        const Insecte* reineAlliee,
                                        const std::map<Hexagon, Insecte*>& plateauSimule) const {
    // Filtrage par distance à la reine adverse
    if (reineAdverse) {
        Hexagon posReineAdv = reineAdverse->getCoords();
        positions.erase(std::remove_if(positions.begin(), positions.end(),
            [&](const Hexagon& h) {
                int dist = std::abs(h.getQ() - posReineAdv.getQ())
                         + std::abs(h.getR() - posReineAdv.getR());
                int maxDistance = static_cast<int>(getPoids("distanceReineAdverse"));
                return dist > maxDistance;

            }),
            positions.end()
        );
    }

    // Filtrage si la reine alliée est "engorgée"
    if (reineAlliee) {
        int nombreVoisins = calculerNombreDeVoisins(plateauSimule, reineAlliee->getCoords());
        if (nombreVoisins > 2) {
            Hexagon posReineAlliee = reineAlliee->getCoords();
            positions.erase(std::remove_if(positions.begin(), positions.end(),
                [&](const Hexagon& h) {
                    int dist = std::abs(h.getQ() - posReineAlliee.getQ())
                             + std::abs(h.getR() - posReineAlliee.getR());
                    int rayonEngorge = static_cast<int>(getPoids("distanceReineAllieeEngorge"));
                    return dist <= rayonEngorge;

                }),
                positions.end()
            );
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

double JoueurIANiveau2::evaluerCohesion(const Joueur* joueur, Insecte* insecte, const Hexagon& emplacement,
                                        const std::map<Hexagon, Insecte*>& plateauSimule) const {
    std::size_t plateauH = hashPlateau(plateauSimule);
    HeuristicKey key{insecte, emplacement, plateauH};

    auto it = heuristicsCache.find(key);
    if (it != heuristicsCache.end()) {
        return it->second;
    }

    double score = 0.0;
    std::vector<Hexagon> voisins = getVoisins(emplacement);
    for (const Hexagon& v : voisins) {
        auto found = plateauSimule.find(v);
        if (found != plateauSimule.end() && found->second && found->second->getOwner() == joueur) {
            score += getPoids("evaluerCohesion");
        }
    }

    // Mise en cache
    heuristicsCache[key] = score;
    return score;
}


double JoueurIANiveau2::evaluerAttaqueReineAdverse(const Joueur* joueur, Insecte* insecte,
                                                   const Hexagon& nouvelEmplacement,
                                                   const std::map<Hexagon, Insecte*>& plateauSimule) const {
    std::size_t plateauH = hashPlateau(plateauSimule);
    HeuristicKey key{insecte, nouvelEmplacement, plateauH};

    auto it = heuristicsCache.find(key);
    if (it != heuristicsCache.end()) {
        // Supposons qu'on a déjà stocké le score d'attaqueReineAdverse
        return it->second;
    }

    double score = 0.0;
    Insecte* reineAdverse = joueur->getReineAdverse(plateauSimule);
    if (reineAdverse) {
        Hexagon posReineAdverse = reineAdverse->getCoords();
        std::vector<Hexagon> voisinsReineAdverse = getVoisins(posReineAdverse);
        Hexagon positionActuelle = insecte->getCoords();

        bool dejaMenacant  = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), positionActuelle) != voisinsReineAdverse.end());
        bool menacantApres = (std::find(voisinsReineAdverse.begin(), voisinsReineAdverse.end(), nouvelEmplacement) != voisinsReineAdverse.end());

        if (menacantApres && !dejaMenacant) {
            score += getPoids("MenacerReineAdverse");
        }
        if (dejaMenacant && !menacantApres) {
            score -= getPoids("plusMenacerReineAdverse");
        }
    }

    heuristicsCache[key] = score;
    return score;
}




double JoueurIANiveau2::evaluerProtectionReine(const Joueur* joueur, Insecte* insecte,
                                               const Hexagon& ancienneEmplacement, const Hexagon& nouvelEmplacement,
                                               const std::map<Hexagon, Insecte*>& plateauSimule) const {

    double score = 0.0;
    Insecte* reineAlliee = joueur->getQueenOnPlateau(plateauSimule);
    if (!reineAlliee) return score;

    Hexagon positionReineAlliee = reineAlliee->getCoords();
    std::vector<Hexagon> voisinsReineAvant = getVoisins(positionReineAlliee);
    std::vector<Hexagon> ennemisAvant = reineAlliee->getVoisinsEnnemis(voisinsReineAvant, plateauSimule);
    double nbEnnemisAvant = (double)ennemisAvant.size();

    // Simuler le coup dans une copie du plateau
    std::map<Hexagon, Insecte*> plateauModifie = plateauSimule;
    plateauModifie.erase(ancienneEmplacement);
    plateauModifie[nouvelEmplacement] = insecte;

    Insecte* reineAllieeSimulee = joueur->getQueenOnPlateau(plateauModifie);
    if (!reineAllieeSimulee) return score;

    bool deplacementReine = (insecte == reineAlliee);
    if (deplacementReine) {
        reineAllieeSimulee->setCoords(nouvelEmplacement);
    }
    insecte->setCoords(nouvelEmplacement);

    Hexagon posReineAllieeApres = reineAllieeSimulee->getCoords();
    std::vector<Hexagon> voisinsReineApres = getVoisins(posReineAllieeApres);
    std::vector<Hexagon> ennemisApres = reineAllieeSimulee->getVoisinsEnnemis(voisinsReineApres, plateauModifie);
    double nbEnnemisApres = (double)ennemisApres.size();

    double difference = nbEnnemisAvant - nbEnnemisApres;

    if (difference > 0) {
        double multiplicateur = (nbEnnemisAvant > 4) ? getPoids("multiDeRisqueSurReine") : 1.0;
        double baseBonus = deplacementReine
            ? getPoids("bougerReine") * difference
            : getPoids("bougerInsectePourProtegerReine") * difference;

        score += baseBonus * multiplicateur;
    } else if (difference < 0) {
        score -= getPoids("laisserReineSecurite") * (-difference);
    }

    insecte->setCoords(ancienneEmplacement);

    return score;
}



double JoueurIANiveau2::evaluerBlocageInsecteImportant(const Joueur* joueur, Insecte* insecte,
                                                       const Hexagon& anciennePos, const Hexagon& nouvelEmplacement,
                                                       const std::map<Hexagon, Insecte*>& plateauSimule) const {
    // Comme précédemment, c’est une simulation. On pourrait essayer un cache, mais complexité accrue.
    // On reste sans cache ici.

    std::vector<Hexagon> voisins = getVoisins(nouvelEmplacement);
    std::vector<Insecte*> insectesImportants;

    for (const Hexagon& v : voisins) {
        auto it = plateauSimule.find(v);
        if (it != plateauSimule.end() && it->second) {
            Insecte* adv = it->second;
            if (adv->getOwner() != joueur && (adv->isQueen() || adv->getNom() == "Fourmi")) {
                std::vector<Hexagon> deplacementsAvant = adv->deplacementsPossibles(plateauSimule);
                if (!deplacementsAvant.empty()) {
                    insectesImportants.push_back(adv);
                }
            }
        }
    }

    if (insectesImportants.empty()) {
        return 0.0;
    }

    std::map<Hexagon, Insecte*> plateauModifie = plateauSimule;
    plateauModifie.erase(anciennePos);
    plateauModifie[nouvelEmplacement] = insecte;
    insecte->setCoords(nouvelEmplacement);

    double totalBonus = 0.0;
    for (Insecte* adv : insectesImportants) {
        std::vector<Hexagon> deplacementsApres = adv->deplacementsPossibles(plateauModifie);
        if (deplacementsApres.empty()) {
            totalBonus += getPoids("bonusBlocage");
        }
    }

    insecte->setCoords(anciennePos);
    return totalBonus;
}



double JoueurIANiveau2::evaluerPlacementAction(const Joueur* joueur,
                                               Insecte* insecte,
                                               const Hexagon& emplacement,
                                               const std::map<Hexagon, Insecte*>& plateauSimule) const {
    std::size_t plateauH = hashPlateau(plateauSimule);
    HeuristicKey key{insecte, emplacement, plateauH};

    auto it = heuristicsCache.find(key);
    if (it != heuristicsCache.end()) {
        return it->second;
    }
    Hexagon anciennePos = insecte->getCoords();

    // Simuler l'ajout de l'insecte
    std::map<Hexagon, Insecte*> plateauCopie = plateauSimule;
    plateauCopie[emplacement] = insecte;
    insecte->setCoords(emplacement);

    double score = evaluerCohesion(joueur, insecte, emplacement, plateauCopie);

    // On évalue le potentiel futur du placement en regardant les déplacements possibles post-placement
    std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateauCopie);
    double meilleurScoreDeplacement = std::numeric_limits<double>::lowest();
    for (const Hexagon& dep : deplacementsPossibles) {
        double sDep = evaluerDeplacementAction(joueur, insecte, dep, plateauCopie);
        if (sDep > meilleurScoreDeplacement) meilleurScoreDeplacement = sDep;
    }

    if (!deplacementsPossibles.empty()) {
        double potentielFutur = meilleurScoreDeplacement + (double)deplacementsPossibles.size();
        score += getPoids("potentielFuturPlacement") * potentielFutur;
    }

    insecte->setCoords(anciennePos);
    heuristicsCache[key] = score;
    return score;
}


double JoueurIANiveau2::evaluerDeplacementAction(const Joueur* joueur,
                                                 Insecte* insecte,
                                                 const Hexagon& nouvelEmplacement,
                                                 const std::map<Hexagon, Insecte*>& plateauSimule) const {
    std::size_t plateauH = hashPlateau(plateauSimule);
    HeuristicKey key{insecte, nouvelEmplacement, plateauH};

    auto it = heuristicsCache.find(key);
    if (it != heuristicsCache.end()) {
        return it->second;
    }

    Hexagon anciennePos = insecte->getCoords();
    insecte->setCoords(nouvelEmplacement);
    double scoreTotal = 0.0;
    scoreTotal += evaluerAttaqueReineAdverse(joueur, insecte, nouvelEmplacement, plateauSimule);
    scoreTotal += evaluerProtectionReine(joueur, insecte, anciennePos, nouvelEmplacement, plateauSimule);
    scoreTotal += evaluerBlocageInsecteImportant(joueur, insecte, anciennePos, nouvelEmplacement, plateauSimule);

    insecte->setCoords(anciennePos);
    heuristicsCache[key] = scoreTotal;
    return scoreTotal;
}




std::vector<std::pair<Hexagon, double>> JoueurIANiveau2::evaluerEtTrierMouvements(const Joueur* joueur, Insecte* insecte,
                                                                                  const std::vector<Hexagon>& options,
                                                                                  bool estPlacement,
                                                                                  const std::map<Hexagon, Insecte*>& plateauSimule) const {
    std::vector<std::pair<Hexagon, double>> mouvementsAvecScore;
    mouvementsAvecScore.reserve(options.size());

    // Directement évaluer chaque coup via les fonctions complètes
    for (const Hexagon& option : options) {
        double score = estPlacement
            ? evaluerPlacementAction(joueur, insecte, option, plateauSimule)
            : evaluerDeplacementAction(joueur, insecte, option, plateauSimule);

        mouvementsAvecScore.emplace_back(option, score);
    }

    // Trier par score décroissant
    std::sort(mouvementsAvecScore.begin(), mouvementsAvecScore.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    return mouvementsAvecScore;
}




std::vector<Hexagon> JoueurIANiveau2::extraireMeilleursMouvements(const std::vector<std::pair<Hexagon, double>>& mouvementsTries,
                                                                  int nombreMax) const {
    std::vector<Hexagon> meilleurs;
    meilleurs.reserve((size_t)std::min((int)mouvementsTries.size(), nombreMax));

    for (int i = 0; i < std::min((int)mouvementsTries.size(), nombreMax); ++i) {
        meilleurs.push_back(mouvementsTries[i].first);
    }
    return meilleurs;
}



double JoueurIANiveau2::calculerScoreMaxParInsecte(const Joueur* joueur, Insecte* insecte,
                                                   bool estPlacement,
                                                   const std::map<Hexagon, Insecte*>& plateauSimule) const {
    auto it = nouveauxCandidats.find(insecte);
    if (it == nouveauxCandidats.end() || it->second.empty()) {
        return std::numeric_limits<double>::lowest();
    }

    double meilleurScore = std::numeric_limits<double>::lowest();
    for (const Hexagon& option : it->second) {
        double score = estPlacement
            ? evaluerPlacementAction(joueur, insecte, option, plateauSimule)
            : evaluerDeplacementAction(joueur, insecte, option, plateauSimule);
        if (score > meilleurScore) {
            meilleurScore = score;
        }
    }
    return meilleurScore;
}



void JoueurIANiveau2::filtrerMeilleursInsectes(const Joueur* joueur,
                                               std::map<Insecte*, std::vector<Hexagon>>& insectesCandidats,
                                               int nombreMeilleursInsectes,
                                               bool estPlacement,
                                               const std::map<Hexagon, Insecte*>& plateauSimule) {
    if (insectesCandidats.empty()) return;

    std::vector<std::pair<Insecte*, double>> insectesAvecScore;
    insectesAvecScore.reserve(insectesCandidats.size());
    for (const auto& [insecte, _] : insectesCandidats) {
        double scoreMax = calculerScoreMaxParInsecte(joueur, insecte, estPlacement, plateauSimule);
        insectesAvecScore.emplace_back(insecte, scoreMax);
    }

    std::sort(insectesAvecScore.begin(), insectesAvecScore.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    std::map<Insecte*, std::vector<Hexagon>> meilleursInsectes;
    for (int i = 0; i < std::min((int)insectesAvecScore.size(), nombreMeilleursInsectes); ++i) {
        Insecte* insecte = insectesAvecScore[i].first;
        meilleursInsectes[insecte] = insectesCandidats[insecte];
    }

    insectesCandidats = std::move(meilleursInsectes);
}

bool JoueurIANiveau2::choisirCoupAleatoire(const Joueur* joueur,
                                           const std::map<Hexagon, Insecte*>& plateauSimule,
                                           const std::vector<Insecte*>& deck,
                                           bool estPlacement) {
    if (estPlacement) {
        // Coup aléatoire de placement
        if (deck.empty()) {
            return false;
        }

        int insecteIndex = randomDeckChoice();
        Insecte* insecte = getInsecteByIndex((unsigned int)insecteIndex);
        if (!insecte) {
            return false;
        }

        std::vector<Hexagon> placementsPossibles = insecte->getPlacementsPossibles(plateauSimule);
        if (placementsPossibles.empty()) {
            return false;
        }

        int placementIndex = randomHexagonIndexChoice(placementsPossibles);
        Hexagon placementChoisi = placementsPossibles[placementIndex - 1];
        candidats[insecte].push_back(placementChoisi);
        actionChoisie = PLACER;
        return true;
    } else {
        // Coup aléatoire de déplacement
        std::vector<Insecte*> insectesDuJoueur = joueur->getInsectesDuJoueur(plateauSimule);
        if (insectesDuJoueur.empty()) {
            return false;
        }

        int insecteIndex = randomPionIndexChoice(insectesDuJoueur);
        Insecte* insecte = insectesDuJoueur[insecteIndex];
        std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateauSimule);
        if (deplacementsPossibles.empty()) {
            return false;
        }

        int deplacementIndex = randomHexagonIndexChoice(deplacementsPossibles);
        Hexagon deplacementChoisi = deplacementsPossibles[deplacementIndex - 1];
        candidats[insecte].push_back(deplacementChoisi);
        actionChoisie = DEPLACER;
        return true;
    }
}




void JoueurIANiveau2::choisirHeuristiquePourPlacer(const Joueur* joueur,
                                                   const std::vector<Insecte*>& deck,
                                                   const std::map<Hexagon, Insecte*>& plateauSimule) {
    // Remplir candidats avec deck + filtrage
    remplirCandidatsAvecDeck(joueur, deck, plateauSimule);

    if (candidats.empty()) {
        if (!choisirCoupAleatoire(joueur, plateauSimule, deck, true)) {
            actionChoisie = AUCUN_ACTION;
        }
        return;
    }


    int nombreMaxPlacements = 1;
    int nombreMeilleursInsectes = 1;
    nouveauxCandidats.clear();

    for (auto& [insecte, placements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(joueur, insecte, placements, true, plateauSimule);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxPlacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    filtrerMeilleursInsectes(joueur, nouveauxCandidats, nombreMeilleursInsectes, true, plateauSimule);
    candidats = std::move(nouveauxCandidats);

    if (candidats.empty()) {
        // Encore aucun candidat après filtrage détaillé, on tente l'aléatoire
        if (!choisirCoupAleatoire(joueur, plateauSimule, deck, true)) {
            // Impossible même en aléatoire
            actionChoisie = AUCUN_ACTION;
        }
    } else {
        actionChoisie = PLACER;
    }
}

void JoueurIANiveau2::choisirHeuristiquePourDeplacer(const Joueur* joueur,
                                                     const std::map<Hexagon, Insecte*>& plateauSimule) {
    // Remplir candidats avec plateau + filtrage
    remplirCandidatsAvecPlateau(joueur, plateauSimule);

    if (candidats.empty()) {
        if (!choisirCoupAleatoire(joueur, plateauSimule, {}, false)) {
            actionChoisie = AUCUN_ACTION;
        }
        return;
    }


    int nombreMaxDeplacements = 1;
    int nombreMeilleursInsectes = 1;
    nouveauxCandidats.clear();

    for (auto& [insecte, deplacements] : candidats) {
        auto mouvementsTries = evaluerEtTrierMouvements(joueur, insecte, deplacements, false, plateauSimule);
        auto meilleurs = extraireMeilleursMouvements(mouvementsTries, nombreMaxDeplacements);
        if (!meilleurs.empty()) {
            nouveauxCandidats[insecte] = std::move(meilleurs);
        }
    }

    filtrerMeilleursInsectes(joueur, nouveauxCandidats, nombreMeilleursInsectes, false, plateauSimule);
    candidats = std::move(nouveauxCandidats);

    if (candidats.empty()) {
        // Plus rien, on essaye l'aléatoire
        if (!choisirCoupAleatoire(joueur, plateauSimule, {}, false)) {
            actionChoisie = AUCUN_ACTION;
        }
    } else {
        actionChoisie = DEPLACER;
    }
}





double JoueurIANiveau2::calculerScoreMoyen(const Joueur* joueur,
                                           const std::map<Insecte*, std::vector<Hexagon>>& candidats,
                                           const std::map<Hexagon, Insecte*>& plateauSimule,
                                           bool estPlacement) const {
    if (candidats.empty()) {
        return 0.0;
    }

    double somme = 0.0;
    int count = 0;

    // Grâce au cache, chaque appel à evaluerPlacementAction ou evaluerDeplacementAction est O(1) après la première fois.
    for (const auto& [insecte, hexs] : candidats) {
        for (const Hexagon& h : hexs) {
            double score = estPlacement
                ? evaluerPlacementAction(joueur, insecte, h, plateauSimule)
                : evaluerDeplacementAction(joueur, insecte, h, plateauSimule);
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
        scoreMoyenPlacement = calculerScoreMoyen(this, candidats, getPlateau(), true);
    }
    std::map<Insecte*, std::vector<Hexagon>> candidatsPlacement = candidats;

    // Réinitialiser et évaluer les déplacements
    reinitialiserAttributs();
    choisirHeuristiquePourDeplacer(this, getPlateau());
    double scoreMoyenDeplacement = 0.0;
    if (!candidats.empty()) {
        scoreMoyenDeplacement = calculerScoreMoyen(this, candidats, getPlateau(), false);
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

void JoueurIANiveau2::mettreAJourCoordonneesInsectes(const std::map<Hexagon, Insecte*>& plateau) const {
    for (auto& [position, insecte] : plateau) {
        if (insecte) {
            insecte->setCoords(position); // Mettre à jour les coordonnées de l'insecte
        }
    }
}

std::size_t JoueurIANiveau2::hashPlateau(const std::map<Hexagon, Insecte*>& plateau) const {
    // Combinateur de hash
    std::size_t seed = 0;
    for (auto& [hex, insecte] : plateau) {
        if (!insecte) continue;
        // Combine coords
        seed ^= std::hash<int>()(hex.getQ()) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>()(hex.getR()) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);

        // Combine identifiant unique ou nom
        std::hash<std::string> hashStr;
        seed ^= hashStr(insecte->getNom()) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    }
    return seed;
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

size_t generateRandomKeyHash() {
    std::uniform_int_distribution<size_t> dist;
    return dist(rng);
}

// Fonction qui initialise une entrée Zobrist pour une case donnée si elle n'existe pas encore
void ensureZobristKeysForHexHash(const Hexagon& h) {
    if (zobristKeys.find(h) == zobristKeys.end()) {
        std::array<size_t, nbTypesInsectes> arr;
        for (int i = 0; i < nbTypesInsectes; ++i) {
            arr[i] = generateRandomKeyHash();
        }
        zobristKeys[h] = arr;
    }
}

size_t calculerZobristHashPlateau(const std::map<Hexagon, Insecte*>& plateau) {
    size_t hash = 0;
    for (auto& [pos, insecte] : plateau) {
        if (!insecte) continue;
        ensureZobristKeysForHexHash(pos);
        int typeId = insecte->getTypeId();
        hash ^= zobristKeys[pos][typeId];
    }
    return hash;
}

int JoueurIANiveau3::getInputForAction(){
    reinitialiserAttributs();

    int meilleurScore = -std::numeric_limits<int>::max();
    std::pair<Insecte*, Hexagon> meilleurCoup{nullptr, Hexagon()};

    // Créer des copies des decks pour éviter de modifier les originaux
    std::vector<Insecte*> deckMax = copierDeck(this);
    std::vector<Insecte*> deckMin = copierDeck(adversaire);
    // Créer une copie locale du plateau à partir de getPlateau()
    std::map<Hexagon, Insecte*> plateauLocal = getPlateau();


    // Générer les coups possibles pour le joueur IA
    auto coupsPossibles = genererCoups(getPlateau(), true, deckMax, deckMin);

    // Si aucun coup possible, retour à une action par défaut
    if (coupsPossibles.empty()) {
        actionChoisie = AUCUN_ACTION;
        return static_cast<int>(actionChoisie);
    }

    // Parcourir les coups possibles pour évaluer leur score
    for (auto& [insecte, positions] : coupsPossibles) {
        for (auto& position : positions) {
            // Sauvegarder l'état initial pour annulation
            Hexagon anciennePosition = insecte->getCoords();
            bool estPlacement = this->estPlacement(getPlateau(), insecte);

            // Simuler le coup sur une copie du plateau
            auto plateauSimule = simulerCoup(plateauLocal, std::make_pair(insecte, position), deckMax, deckMin, true);

            // Calculer le score en appelant minimax
            int score = minimax(plateauSimule, 10, false, -std::numeric_limits<int>::max(),
                                std::numeric_limits<int>::max(), deckMax, deckMin);

            undoCoup(plateauLocal, insecte, anciennePosition, position, estPlacement, deckMax, deckMin, true);


            // Mettre à jour le meilleur coup si le score est supérieur
            if (score > meilleurScore) {
                meilleurScore = score;
                meilleurCoup = {insecte, position};
            }
        }
    }

    reinitialiserAttributs();

    // Ajouter le meilleur coup aux candidats
    candidats[meilleurCoup.first].push_back(meilleurCoup.second);

    // Déterminer l'action choisie (placement ou déplacement)
    bool placement = estPlacement(getPlateau(), meilleurCoup.first);
    actionChoisie = placement ? PLACER : DEPLACER;

    mettreAJourCoordonneesInsectes(getPlateau());

    return static_cast<int>(actionChoisie);
}

std::map<Hexagon, Insecte*> JoueurIANiveau3::simulerCoup(std::map<Hexagon, Insecte*>& plateauSimule,
                                                         const std::pair<Insecte*, Hexagon>& coup,
                                                         std::vector<Insecte*>& deckMaximisateur,
                                                         std::vector<Insecte*>& deckMinimisateur,
                                                         bool estMaximisant) {
    Insecte* insecte = coup.first;
    const Hexagon& nouvellePosition = coup.second;

    if (estPlacement(plateauSimule, insecte)) {
        std::vector<Insecte*>& deckActuel = estMaximisant ? deckMaximisateur : deckMinimisateur;
        auto it = std::find(deckActuel.begin(), deckActuel.end(), insecte);
        if (it != deckActuel.end()) {
            deckActuel.erase(it);
        } else {
            throw std::runtime_error("Insecte non trouvé dans le deck pour le placement.");
        }
    } else {
        // Déplacement
        Hexagon anciennePosition = insecte->getCoords();
        plateauSimule.erase(anciennePosition);
    }

    plateauSimule[nouvellePosition] = insecte;
    insecte->setCoords(nouvellePosition);

    return plateauSimule;
}




int JoueurIANiveau3::minimax(std::map<Hexagon, Insecte*> plateau,
                             int profondeur,
                             bool maximisateur,
                             int alpha,int beta,
                             std::vector<Insecte*>& deckMax,
                             std::vector<Insecte*>& deckMin)
{
    // Calcul du zobristHash pour l’état actuel
    size_t zobrist = calculerZobristHashPlateau(plateau);

    // On crée les copies triées
    std::vector<Insecte*> deckMaxCopie = deckMax;
    std::vector<Insecte*> deckMinCopie = deckMin;

    std::sort(deckMaxCopie.begin(), deckMaxCopie.end(), [](Insecte* a, Insecte* b){
        return a->getNom() < b->getNom();
    });
    std::sort(deckMinCopie.begin(), deckMinCopie.end(), [](Insecte* a, Insecte* b){
        return a->getNom() < b->getNom();
    });

    GameState state{zobrist, deckMaxCopie, deckMinCopie};

    auto it = transpositionTable.find(state);
    if (it != transpositionTable.end()) {
        // on retourne le score stocké dans la TT
        return it->second.score;
    }

    auto coupsMax = genererCoups(plateau, true, deckMax, deckMin);
    auto coupsMin = genererCoups(plateau, false, deckMax, deckMin);

    bool pasDeCoupMax = coupsMax.empty();
    bool pasDeCoupMin = coupsMin.empty();

    if (profondeur <= 0 || (pasDeCoupMax && pasDeCoupMin) || estTerminal(plateau, deckMax, deckMin)) {
        int scoreTerminal = evaluerPlateau(plateau, deckMax, deckMin);
        TranspositionEntry entry;
        entry.score = scoreTerminal;
        entry.profondeur = profondeur;
        entry.type = TranspositionEntry::EXACT;  // par exemple
        transpositionTable[state] = entry;
        return scoreTerminal;
    }

    if (maximisateur) {
        if (coupsMax.empty()) {
            int scoreFin = evaluerPlateau(plateau, deckMax, deckMin);
            TranspositionEntry entry;
            entry.score = scoreFin;
            entry.profondeur = profondeur;
            entry.type = TranspositionEntry::EXACT;
            transpositionTable[state] = entry;
            return scoreFin;
        }
        int meilleurScore = -std::numeric_limits<int>::max();

        for (auto& [insecte, positions] : coupsMax) {
            for (auto& position : positions) {
                Hexagon anciennePosition = insecte->getCoords();
                bool isPlacement = estPlacement(plateau, insecte);

                auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position), deckMax, deckMin, true);

                int score = minimax(plateauSimule, profondeur - 1, false, alpha, beta, deckMax, deckMin);

                undoCoup(plateau, insecte, anciennePosition, position, isPlacement, deckMax, deckMin, true);

                meilleurScore = std::max(meilleurScore, score);
                alpha = std::max(alpha, score);
                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;
        }

        TranspositionEntry entry;
        entry.score = meilleurScore;
        entry.profondeur = profondeur;
        entry.type = TranspositionEntry::EXACT;
        transpositionTable[state] = entry;
        return meilleurScore;
    } else {
        if (coupsMin.empty()) {
            int scoreFin = evaluerPlateau(plateau, deckMax, deckMin);
            TranspositionEntry entry;
            entry.score = scoreFin;
            entry.profondeur = profondeur;
            entry.type = TranspositionEntry::EXACT;
            transpositionTable[state] = entry;
            return scoreFin;
        }
        int meilleurScore = std::numeric_limits<int>::max();

        for (auto& [insecte, positions] : coupsMin) {
            for (auto& position : positions) {
                Hexagon anciennePosition = insecte->getCoords();
                bool isPlacement = estPlacement(plateau, insecte);

                auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position), deckMax, deckMin, false);

                int score = minimax(plateauSimule, profondeur - 1, true, alpha, beta, deckMax, deckMin);

                undoCoup(plateau, insecte, anciennePosition, position, isPlacement, deckMax, deckMin, false);

                meilleurScore = std::min(meilleurScore, score);
                beta = std::min(beta, score);
                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;
        }

        TranspositionEntry entry;
        entry.score = meilleurScore;
        entry.profondeur = profondeur;
        entry.type = TranspositionEntry::EXACT;
        transpositionTable[state] = entry;
        return meilleurScore;
    }
}




int JoueurIANiveau3::evaluerPlateau(const std::map<Hexagon, Insecte*>& plateau,
                                    const std::vector<Insecte*>& deckMaximisateur,
                                    const std::vector<Insecte*>& deckMinimisateur) {
    Insecte* reineAlliee = getQueenOnPlateau(plateau);
    Insecte* reineAdverse = getReineAdverse(plateau);

    int score = 0;
    // Vérification encerclement
    if (reineAdverse && estEncerclee(reineAdverse, plateau)) {
        score += 1000;
    }
    if (reineAlliee && estEncerclee(reineAlliee, plateau)) {
        score -= 1000;
    }

    // Avantage matériel
    int nbAllies = 0;
    int nbEnnemis = 0;
    for (auto& [pos, insecte] : plateau) {
        if (!insecte) continue;
        if (insecte->getOwner() == this) nbAllies++;
        else nbEnnemis++;
    }

    nbAllies += (int)deckMaximisateur.size();
    nbEnnemis += (int)deckMinimisateur.size();

    score += (nbAllies - nbEnnemis) * 10;

    // Menaces sur les reines
    // Menacer la reine adverse est bon pour nous, donc +score
    if (reineAdverse) {
        int menacesSurReineAdverse = 0;
        std::vector<Hexagon> voisinsReineAdv = getVoisins(reineAdverse->getCoords());
        for (const Hexagon& v : voisinsReineAdv) {
            auto it = plateau.find(v);
            if (it != plateau.end() && it->second && it->second->getOwner() == this) {
                menacesSurReineAdverse++;
            }
        }
        // On peut par exemple ajouter 20 points par menace
        score += menacesSurReineAdverse * 20;
    }

    // Menaces sur notre reine alliée (ennemis autour)
    if (reineAlliee) {
        int menacesSurReineAlliee = 0;
        std::vector<Hexagon> voisinsReineAlliee = getVoisins(reineAlliee->getCoords());
        for (const Hexagon& v : voisinsReineAlliee) {
            auto it = plateau.find(v);
            if (it != plateau.end() && it->second && it->second->getOwner() != this) {
                menacesSurReineAlliee++;
            }
        }
        // Pour chaque menace ennemie sur notre reine, on peut enlever 20 points
        score -= menacesSurReineAlliee * 20;
    }

    return score;
}



bool JoueurIANiveau3::estTerminal(const std::map<Hexagon, Insecte*>& plateau,
                                  const std::vector<Insecte*>& deckMaximisateur,
                                  const std::vector<Insecte*>& deckMinimisateur) {
    // Une partie est terminale si la reine alliée est encerclée ou si la reine adverse est encerclée.
    Insecte* reineAlliee = getQueenOnPlateau(plateau);
    if (reineAlliee && estEncerclee(reineAlliee, plateau)) {
        return true; // Défaite pour nous, mais terminal
    }

    Insecte* reineAdverse = getReineAdverse(plateau);
    if (reineAdverse && estEncerclee(reineAdverse, plateau)) {
        return true; // Victoire, terminal
    }

    return false;
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
    Hexagon positionReine = reine->getCoords();

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

void JoueurIANiveau3::undoCoup(std::map<Hexagon, Insecte*>& plateau,
                               Insecte* insecte,
                               const Hexagon& anciennePosition,
                               const Hexagon& nouvellePosition,
                               bool estPlacement,
                               std::vector<Insecte*>& deckMax,
                               std::vector<Insecte*>& deckMin,
                               bool maximisateur) {

    plateau.erase(nouvellePosition);

    if (estPlacement) {
        auto& deckActuel = maximisateur ? deckMax : deckMin;
        deckActuel.push_back(insecte);
    } else {
        plateau[anciennePosition] = insecte;
    }

    insecte->setCoords(anciennePosition);
}



