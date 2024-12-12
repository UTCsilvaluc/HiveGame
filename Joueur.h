#ifndef JOUEUR_H
#define JOUEUR_H

#include "Insecte.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <stdexcept>
#include <map>
#include <limits>

int getInput(const std::string& prompt, int minValue, int maxValue, unsigned int tour);
int getInput(const std::string& prompt, int minValue, int maxValue);
class Insecte;
class Joueur;
std::vector<Insecte*> deckDeBase(Joueur *joueur);

class Joueur{
private:
    std::string nom;
    std::vector<Insecte*> deck;
public:
    Joueur(const std::string& nom) : nom(nom), deck(deckDeBase(this)) {}
    Joueur(std::string n, std::vector<Insecte*> d) {
        if (n.empty()) { throw std::invalid_argument("Le nom ne peut pas être vide."); }
        nom = n;
        deck = d.empty() ? deckDeBase(this) : d;
    }

    const std::string &getName() const { return nom; }
    const std::vector<Insecte*>& getDeck() const { return deck; }
    Insecte* getQueen() const;
    Insecte* getQueenOnPlateau(const std::map<Hexagon, Insecte*>& plateau) const;
    int findInsectIndexInDeck(const std::vector<Insecte*>& deck, Insecte* insecte);
    Insecte* getReineAdverse(const std::map<Hexagon, Insecte*>& plateau) const;

    int getQueenIndex() const;
    bool hasQueen() const;
    void afficherDeck() const;
    std::string toJson() const;
    virtual ~Joueur() {
        for (Insecte* insecte : deck) {
            delete insecte;
        }
    }

    size_t getDeckSize() const {
        return deck.size();
    }

    Insecte* getInsecteByIndex(unsigned int i) {
        if (i >= deck.size()) {return nullptr; }
        return deck[i];
    }

    void retirerInsecte(unsigned int index) {
        if (index < deck.size()) {
            deck.erase(deck.begin() + index);
        } else {
            std::cout << "Index invalide. Aucune action effectuée." << std::endl;
        }
    }

    Insecte* contientInsecte(const std::string& nomInsecte) const;

    void ajouterInsecte(Insecte* insecte) ;

    std::vector<Insecte*> getInsectesDuJoueur(const std::map<Hexagon, Insecte*>& plateauMap) const ;

    // Méthodes virtuelles pures
    virtual int getInputForAction() = 0; // Choix entre déplacer, placer ou annuler
    virtual Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour) = 0; // Coordonnées du premier placement
    virtual int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) = 0; // Choisir un insecte à déplacer
    virtual int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) = 0; // Choisir un mouvement pour un insecte
    virtual int getInputForDeckIndex() = 0; // Choisir un insecte du deck à placer
    virtual int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) = 0; // Choisir un emplacement pour placer un insecte
};

class JoueurHumain : public Joueur{

public:
    JoueurHumain(const std::string& nom) : Joueur(nom) {}

    int getInputForAction() {
        int choice = 0;
        while (true) {
            std::cout << "Que voulez-vous faire ?\n"
                      << "1 - D�placer un pion \n"
                      << (getDeckSize() > 0 ? "2 - Placer un pion \n" : "")
                      << "3 - Retour arri�re (Annuler la derni�re action)\n";
            std::cin >> choice;

            if (std::cin.fail() || (choice != 1 && (choice != 2 || getDeckSize() == 0)) && choice != 3) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Merci de saisir 1, 2 (si deck disponible), ou 3 pour retourner en arri�re.\n";
            } else {
                break; // Sortir de la boucle si l'entr�e est valide
            }
        }
        return choice;
    }

    Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour){
        std::cout << "Le plateau est vide, vous devez entrer les coordonnées directement.\n";
        int x = getInput("Abscisse pour poser le pion : ", minQ - 1, maxQ + 1 , tour);
        int y = getInput("Ordonnée pour poser le pion : ", minR - 1, maxR + 1 , tour);
        return Hexagon(x,y);
    }

    int getInputForDeckIndex(){
        return getInput("Quel pion souhaitez-vous poser ? ", 1, getDeckSize()) - 1;
    }

    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles){
        return getInput("Choisissez un emplacement ou entrez -1 pour annuler : ", -1, placementsPossibles.size());
    }

    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur){
        return getInput("Entrez l'index de l'insecte à sélectionner (ou -1 pour annuler) : ", -1, insectesDuJoueur.size() - 1);
    }

    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles){
        return getInput("Choisissez un emplacement ou entrez -1 pour annuler : ", -1, deplacementsPossibles.size());
    }

};






class JoueurIA : public Joueur {
private:
    std::default_random_engine generator;  // Générateur de nombres aléatoires

public:
    JoueurIA(const std::string& nom) : Joueur(nom) {
        // Initialiser le générateur de nombres aléatoires avec la graine actuelle
        std::random_device rd;
        generator = std::default_random_engine(rd());
    }

    int getInputForAction() {
        return randomChoice();
    }

    Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour){
        // À implémenter si on veut faire commencer IA ou faire jouer IA contre IA
        return Hexagon(0,0);
    }

    int getInputForDeckIndex() {
        return randomDeckChoice();
    }

    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) {
        return randomHexagonIndexChoice(placementsPossibles);
    }

    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) {
        return randomPionIndexChoice(insectesDuJoueur);
    }

    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) {
        return randomHexagonIndexChoice(deplacementsPossibles);
    }

    // Fonction pour choisir aléatoirement entre "poser" ou "déplacer"
    int randomChoice() {
        std::uniform_int_distribution<int> distribution(1, 2);  // Distribution entre 1 et 2
        return distribution(generator);  // Retourne 1 ("poser") ou 2 ("déplacer")
    }

    // Fonction pour choisir un Hexagon aléatoire parmi les options de déplacement disponibles
    int randomHexagonIndexChoice(const std::vector<Hexagon>& options) {
        if (options.empty()) {
            throw std::runtime_error("Aucune option possible");
        }
        std::uniform_int_distribution<size_t> distribution(1, options.size());
        return distribution(generator);
    }

    // Fonction pour choisir un Insecte aléatoire du deck du joueur
    int randomDeckChoice() {
        if (getDeckSize() == 0) {
            throw std::runtime_error("Deck vide");
        }
        std::uniform_int_distribution<int> distribution(1, getDeckSize());
        return distribution(generator);
    }

    // Fonction pour choisir aléatoirement un index pion du plateau appartenant au joueur
    int randomPionIndexChoice(std::vector<Insecte*> insectesDuJoueur) {
        if (insectesDuJoueur.empty()) {
            throw std::runtime_error("Aucun pion appartenant au joueur sur le plateau");
        }
        std::uniform_int_distribution<int> distribution(0, insectesDuJoueur.size() - 1);
        return distribution(generator);
    }

    // Nouvelle fonction : Générer un float aléatoire entre min et max
    float randomFloat(float min = 0.0, float max = 1.0) {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

    int randomIndex(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }
};








enum HeuristiqueType { PROTEGER_REINE, ATTAQUER_REINE, COMPACTER_RUCHE, AUCUN_HEURISTIQUE }; // Ajout d'une énumération pour les types d'heuristiques

enum ActionType { DEPLACER = 1, PLACER = 2, AUCUN_ACTION = 0 };

class JoueurIANiveau2 : public JoueurIA {
protected:
    ActionType actionChoisie;                           // PLACER, DEPLACER, AUCUN
    Hexagon positionChoisie;                            // Pour mémoriser la position choisie
    Insecte* insecteChoisi;                             // Pour mémoriser l'insecte choisi (peut être dans le deck ou sur le plateau)
    std::map<Insecte*, std::vector<Hexagon>> nouveauxCandidats; // Nouveaux candidats pour chaque heuristique
    std::map<Insecte*, std::vector<Hexagon>> candidats; // Map pour stocker les insectes et leurs déplacements possibles
    std::vector<HeuristiqueType> historiqueHeuristiques; // Historique des heuristiques choisies
    const std::map<Hexagon, Insecte*>* plateau;         // Pointeur vers le plateau
    unsigned int* tour;                                 // Pointeur vers le numéro du tour

    // Fonctions de service internes

    // Évalue un placement pour un insecte, retourne un score
    int evaluerPlacementAction(Insecte* insecte, const Hexagon& emplacement, const std::map<Hexagon, Insecte*>& plateau) const;
    // Évalue un déplacement pour un insecte, retourne un score
    int evaluerDeplacementAction(Insecte* insecte, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const;
    // Tri et renvoie un vecteur (Hexagon, int) représentant l'emplacement/le déplacement et son score
    std::vector<std::pair<Hexagon,int>> evaluerEtTrierMouvements(Insecte* insecte, const std::vector<Hexagon>& options, bool estPlacement) const;
    // Extrait les meilleurs mouvements (placements ou déplacements) à partir d'un vecteur trié
    std::vector<Hexagon> extraireMeilleursMouvements(const std::vector<std::pair<Hexagon,int>>& mouvementsTries, int nombreMax) const;
    // Calcule le score maximum d'un insecte en fonction des mouvements stockés dans nouveauxCandidats
    int calculerScoreMaxParInsecte(Insecte* insecte, bool estPlacement) const;
    // Filtre les meilleurs insectes en fonction de leurs scores max
    void filtrerMeilleursInsectes(std::map<Insecte*, std::vector<Hexagon>>& insectesCandidats, int nombreMeilleursInsectes, bool estPlacement);
    int evaluerAttaqueReineAdverse(Insecte* insecte, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const;
    int evaluerProtectionReine(Insecte* insecte, const Hexagon& ancienEmplacement, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const;

public:
    // Constructeur mis à jour pour inclure le plateau et le tour
    JoueurIANiveau2(std::string nom, const std::map<Hexagon, Insecte*>* plateauRef, unsigned int* tourRef)
        : JoueurIA(nom),
          actionChoisie(AUCUN_ACTION),
          positionChoisie(),
          insecteChoisi(nullptr),
          historiqueHeuristiques(),
          plateau(plateauRef),
          tour(tourRef) {}

    // Getter pour le plateau
    const std::map<Hexagon, Insecte*>& getPlateau() const { return *plateau; }

    // Getter pour le tour
    unsigned int getTour() const { return *tour; }
    Hexagon getPositionChoisie() const { return positionChoisie; }
    Insecte* getInsecteChoisi() const { return insecteChoisi; }
    // Getter pour l'attribut candidats
    const std::map<Insecte*, std::vector<Hexagon>>& getCandidats() const {return candidats;}

    // Setter pour l'attribut candidats
    void setCandidats(const std::map<Insecte*, std::vector<Hexagon>>& nouveauxCandidats) {candidats = nouveauxCandidats;}

    int evaluerDeplacement();
    int evaluerPlacement();

    void choisirHeuristiquePourDeplacer();
    void choisirHeuristiquePourPlacer();

    void reinitialiserAttributs();
    void afficherCandidats() const;
    void remplirCandidatsAvecDeck();
    void filtrerPrioriteFourmies();
    int evaluerCohesion(const Hexagon& emplacement) const;
    int evaluerBlocageInsecteImportant(Insecte* insecte, const Hexagon& anciennePos, const Hexagon& nouvelEmplacement, const std::map<Hexagon, Insecte*>& plateau) const;
    double calculerScoreMoyenDeDeplacement(const std::map<Insecte*, std::vector<Hexagon>>& candidats) const;
    double calculerScoreMoyenDePlacement(const std::map<Insecte*, std::vector<Hexagon>>& candidats) const;

    void evaluerPlacements(std::map<Insecte*, std::vector<Hexagon>>& candidats, int nombreMaxPlacements);
    void evaluerDeplacements(std::map<Insecte*, std::vector<Hexagon>>& candidats, int nombreMaxDeplacements);

    void remplirCandidatsAvecPlateau();

    static size_t tailleDeckInitiale() {
        // Utiliser une classe dérivée pour l'instance temporaire
        JoueurHumain joueurTemporaire("temp"); // JoueurHumain est une implémentation concrète de Joueur
        std::vector<Insecte*> deckTemporaire = deckDeBase(&joueurTemporaire);
        return deckTemporaire.size();
    }

    int findIndexInOptions(Insecte* insecteChoisi, const std::map<Insecte*, std::vector<Hexagon>>& candidats, const std::vector<Hexagon>& options);


    int getInputForAction() override;
    Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour) override{
        //A implémenter si on veut faire commencer IA ou faire jouer IA contre IA
        return Hexagon(0,0);
    }
    int getInputForDeckIndex() override;
    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) override;
    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) override;
    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) override;
};

class JoueurIANiveau3 : public JoueurIANiveau2 {
public:
    JoueurIANiveau3(std::string nom, const std::map<Hexagon, Insecte*>* plateauRef, unsigned int* tourRef, Joueur* adversaireRef)
        : JoueurIANiveau2(nom, plateauRef, tourRef), adversaire(adversaireRef) {}



    // Surcharger les mÃ©thodes pures de Joueur
    int getInputForAction() override {
        reinitialiserAttributs();
        std::cout<<"nul";
        // On utilise minimax pour dÃ©cider du meilleur coup
        int meilleurScore = -std::numeric_limits<int>::max();
        std::pair<Insecte*, Hexagon> meilleurCoup{nullptr, Hexagon()};
        std::cout<<"aba";

        // GÃ©nÃ©ration de tous les coups possibles pour l'IA (maximisateur)
        auto coupsPossibles = genererCoups(getPlateau(), true);

        std::cout<<"2";

        // Si aucun coup possible, fallback (pas forcÃ©ment utile si estTerminal() gÃ©rÃ©)
        if (coupsPossibles.empty()) {
            actionChoisie = AUCUN_ACTION;
            return static_cast<int>(actionChoisie);
        }

        // Parcourir tous les coups possibles et Ã©valuer via minimax
        for (auto& [insecte, positions] : coupsPossibles) {
            for (auto& position : positions) {
                auto plateauSimule = simulerCoup(getPlateau(), std::make_pair(insecte, position));
                std::cout<<"4_";
                int score = minimax(plateauSimule, 3, false, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
                std::cout<<"5";
                if (score > meilleurScore) {
                    meilleurScore = score;
                    meilleurCoup = {insecte, position};
                }
            }
        }
        std::cout<<"3";

        // Ajouter le meilleur coup aux candidats
        candidats[meilleurCoup.first].push_back(meilleurCoup.second);

        // DÃ©terminer l'action en fonction de si l'insecte Ã©tait dÃ©jÃ  placÃ© ou non
        bool placement = estPlacement(getPlateau(), meilleurCoup.first);
        actionChoisie = placement ? PLACER : DEPLACER;

        std::cout<<"4";

        return static_cast<int>(actionChoisie);
    }

    int getInputForDeckIndex() override {
        return JoueurIANiveau2::getInputForDeckIndex();
    }

    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) override {
        return JoueurIANiveau2::getInputForPlacementIndex(placementsPossibles);
    }

    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) override {
        return JoueurIANiveau2::getInputIndexForInsectToMove(insectesDuJoueur);
    }

    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) override {
        return JoueurIANiveau2::getInputForMovementIndex(deplacementsPossibles);
    }

protected:
    Joueur* adversaire; // Pointeur vers l'adversaire

    int minimax(std::map<Hexagon, Insecte*> plateau, int profondeur, bool maximisateur, int alpha, int beta) {
        // Cas de base : Retourner le score de la fonction d'Ã©valuation
        if (profondeur <= 0 || estTerminal(plateau)) {
            return evaluerPlateau(plateau); // Fonction d'Ã©valuation
        }
        std::cout<<"10";

        if (maximisateur) { // On utilise dÃ©sormais "maximiseur" -> "maximisateur"
            // Maximisation : L'IA joue
            int meilleurScore = -std::numeric_limits<int>::max();
            auto coupsPossibles = genererCoups(plateau, true); // Coups possibles pour l'IA
            std::cout<<"11";
            for (auto& [insecte, positions] : coupsPossibles) {
                for (auto& position : positions) {
                    auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position)); // Simuler le coup
                    int score = minimax(plateauSimule, profondeur - 1, false, alpha, beta);
                    std::cout<<"12";
                    meilleurScore = std::max(meilleurScore, score);
                    alpha = std::max(alpha, score);
                    if (beta <= alpha) {
                        break; // Coupure bÃªta
                    }
                }
                if (beta <= alpha) {
                    break; // Coupure bÃªta
                }
            }
            return meilleurScore;
        } else {
            // Minimisation : L'adversaire joue
            int meilleurScore = std::numeric_limits<int>::max();
            auto coupsPossibles = genererCoups(plateau, false); // Coups possibles pour l'adversaire
            std::cout<<"13";
            for (auto& [insecte, positions] : coupsPossibles) {
                for (auto& position : positions) {
                    auto plateauSimule = simulerCoup(plateau, std::make_pair(insecte, position)); // Simuler le coup
                    int score = minimax(plateauSimule, profondeur - 1, true, alpha, beta);
                    std::cout<<"14";
                    meilleurScore = std::min(meilleurScore, score);
                    beta = std::min(beta, score);
                    if (beta <= alpha) {
                        break; // Coupure alpha
                    }
                }
                if (beta <= alpha) {
                    break; // Coupure alpha
                }
            }
            return meilleurScore;
        }
    }

    int evaluerPlateau(const std::map<Hexagon, Insecte*>& plateau) {
        // Sauvegarder l'Ã©tat initial
        auto sauvegardeCandidats = candidats;
        auto sauvegardeNouveauxCandidats = nouveauxCandidats;
        const std::map<Hexagon, Insecte*>* plateauInitial = this->plateau;

        // Ici, au lieu de rediriger this->plateau vers &plateau (qui est un paramÃ¨tre local),
        // travaillez directement avec "plateau" passÃ© en paramÃ¨tre. Pas besoin de modifier this->plateau.
        // Si vraiment nÃ©cessaire, faites une copie sÃ©parÃ©e, mais Ã©vitez this->plateau = &plateau;
        // qui peut crÃ©er des problÃ¨mes de durÃ©e de vie.

        // Ã‰valuer placements
        reinitialiserAttributs();
        choisirHeuristiquePourPlacer();
        double scoreMoyenPlacement = 0.0;
        if (!candidats.empty()) {
            scoreMoyenPlacement = calculerScoreMoyenDePlacement(candidats);
        }

        // Ã‰valuer dÃ©placements
        reinitialiserAttributs();
        choisirHeuristiquePourDeplacer();
        double scoreMoyenDeplacement = 0.0;
        if (!candidats.empty()) {
            scoreMoyenDeplacement = calculerScoreMoyenDeDeplacement(candidats);
        }

        // Restauration de l'Ã©tat initial
        this->plateau = plateauInitial;
        candidats = sauvegardeCandidats;
        nouveauxCandidats = sauvegardeNouveauxCandidats;

        reinitialiserAttributs();

        return static_cast<int>((scoreMoyenPlacement + scoreMoyenDeplacement) / 2.0);
    }

    // Ajouter le prÃ©fixe JoueurIANiveau3:: aux mÃ©thodes membres
    bool estTerminal(const std::map<Hexagon, Insecte*>& plateau){
        Insecte* reineAlliee = getQueenOnPlateau(plateau);
        Insecte* reineAdverse = getReineAdverse(plateau);

        if (reineAlliee && estEncerclee(reineAlliee, plateau)) {
            return true; // DÃ©faite
        }

        if (reineAdverse && estEncerclee(reineAdverse, plateau)) {
            return true; // Victoire
        }

        // VÃ©rifier si plus aucun coup n'est possible (Ã©galitÃ©)
        std::map<Insecte*, std::vector<Hexagon>> candidatsPlacement;
        std::map<Insecte*, std::vector<Hexagon>> candidatsDeplacement;

        remplirCandidatsAvecDeck();
        candidatsPlacement = candidats;

        remplirCandidatsAvecPlateau();
        candidatsDeplacement = candidats;

        if (candidatsPlacement.empty() && candidatsDeplacement.empty()) {
            return true; // Ã‰galitÃ©
        }

        return false;
    }

    bool estEncerclee(Insecte* reine, const std::map<Hexagon, Insecte*>& plateau) const {
        std::vector<Hexagon> voisins = reine->getCoords().getVoisins();
        for (const Hexagon& voisin : voisins) {
            auto it = plateau.find(voisin);
            if (it == plateau.end() || it->second == nullptr) {
                return false; // Au moins un voisin est libre
            }
        }
        return true;
    }

    std::map<Hexagon, Insecte*> simulerCoup(const std::map<Hexagon, Insecte*>& plateau,
                                            const std::pair<Insecte*, Hexagon>& coup) {
        std::map<Hexagon, Insecte*> plateauSimule = plateau;

        Insecte* insecte = coup.first;
        const Hexagon& nouvellePosition = coup.second;
        bool estUnPlacement = estPlacement(plateauSimule, insecte);

        if (estUnPlacement) {
            plateauSimule[nouvellePosition] = insecte;
        } else {
            Hexagon anciennePosition;
            bool trouve = false;
            for (const auto& [position, insectePlateau] : plateau) {
                if (insectePlateau == insecte) {
                    anciennePosition = position;
                    trouve = true;
                    break;
                }
            }
            if (!trouve && !estUnPlacement) {
                throw std::runtime_error("Insecte non trouvé sur le plateau alors que ce n'est pas un placement.");
            }
        }
        return plateauSimule;
    }


std::map<Insecte*, std::vector<Hexagon>> genererCoups(const std::map<Hexagon, Insecte*>& plateau, bool estMaximisant) {
    // RÃ©initialiser les attributs pour Ã©viter toute interfÃ©rence
    reinitialiserAttributs();

    std::map<Insecte*, std::vector<Hexagon>> coupsPossibles;

    if (estMaximisant) {
        // IA Maximisant
        // GÃ©nÃ©rer les placements
        choisirHeuristiquePourPlacer();
        std::map<Insecte*, std::vector<Hexagon>> placements = getCandidats();

        // RÃ©initialiser les attributs pour gÃ©nÃ©rer les dÃ©placements
        reinitialiserAttributs();

        // GÃ©nÃ©rer les dÃ©placements
        choisirHeuristiquePourDeplacer();
        std::map<Insecte*, std::vector<Hexagon>> deplacements = getCandidats();

        // Fusionner placements et dÃ©placements
        coupsPossibles.insert(placements.begin(), placements.end());
        coupsPossibles.insert(deplacements.begin(), deplacements.end());
    } else {
        // Adversaire Minimisant
        coupsPossibles = genererCoupsAdversaire(plateau);
    }

    return coupsPossibles;
}


std::map<Insecte*, std::vector<Hexagon>> genererCoupsAdversaire(const std::map<Hexagon, Insecte*>& plateau) {
    // Carte pour stocker les coups possibles de l'adversaire
    std::map<Insecte*, std::vector<Hexagon>> coupsAdversaire;

    // Ã‰tape 1 : Identifier tous les insectes adverses sur le plateau
    std::vector<Insecte*> insectesAdversaires;
    for (const auto& [position, insecte] : plateau) {
        if (insecte->getOwner() != this) { // Si l'insecte appartient Ã  l'adversaire
            insectesAdversaires.push_back(insecte);
        }
    }

    // Ã‰tape 2 : GÃ©nÃ©rer tous les dÃ©placements possibles pour chaque insecte adverse
    for (Insecte* insecte : insectesAdversaires) {
        std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateau);
        if (!deplacementsPossibles.empty()) {
            coupsAdversaire[insecte] = deplacementsPossibles;
        }
    }

    // Ã‰tape 3 : Ajouter les placements possibles (depuis le deck adverse)
    std::vector<Insecte*> deckAdversaire = adversaire->getDeck();
    for (Insecte* insecte : deckAdversaire) {
        std::vector<Hexagon> placementsPossibles = insecte->getPlacementsPossibles(plateau);
        if (!placementsPossibles.empty()) {
            coupsAdversaire[insecte] = placementsPossibles;
        }
    }

    return coupsAdversaire;
}



bool estPlacement(const std::map<Hexagon, Insecte*>& plateau, Insecte* insecte) {
    // VÃ©rifie si l'insecte n'est pas encore sur le plateau
    for (const auto& [position, insectePlateau] : plateau) {
        if (insectePlateau == insecte) {
            return false; // L'insecte est dÃ©jÃ  placÃ©
        }
    }
    return true; // L'insecte n'est pas encore placÃ©
}



};





#endif // JOUEUR_H
