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
#include <stack>

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






class JoueurIANiveau2 : public JoueurIA {
protected:
    enum ActionType { DEPLACER = 1, PLACER = 2, AUCUN_ACTION = 0 };

    ActionType actionChoisie;                           // PLACER, DEPLACER, AUCUN
    Hexagon positionChoisie;                            // Pour mémoriser la position choisie
    Insecte* insecteChoisi;                             // Pour mémoriser l'insecte choisi (peut être dans le deck ou sur le plateau)
    std::map<Insecte*, std::vector<Hexagon>> nouveauxCandidats; // Nouveaux candidats pour chaque heuristique
    std::map<Insecte*, std::vector<Hexagon>> candidats; // Map pour stocker les insectes et leurs déplacements possibles
    const std::map<Hexagon, Insecte*>* plateau;         // Pointeur vers le plateau
    unsigned int* tour;                                 // Pointeur vers le numéro du tour

public:
    JoueurIANiveau2(const std::string& nom, const std::map<Hexagon, Insecte*>* plateauRef, unsigned int* tourRef)
        : JoueurIA(nom), plateau(plateauRef), tour(tourRef) {}

    // Getter pour nouveauxCandidats
    const std::map<Insecte*, std::vector<Hexagon>>& getNouveauxCandidats() const {
        return nouveauxCandidats;
    }

    // Getter pour candidats
    const std::map<Insecte*, std::vector<Hexagon>>& getCandidats() const {
        return candidats;
    }

    // Getter pour plateau
    const std::map<Hexagon, Insecte*> getPlateau() const {
        return *plateau;
    }

    // Getter pour tour
    unsigned int getTour() const {
        return *tour;
    }

    // Remplit les candidats avec les insectes disponibles dans le deck
    void remplirCandidatsAvecDeck(const Joueur* joueur,
                                  const std::vector<Insecte*>& deck,
                                  const std::map<Hexagon, Insecte*>& plateauSimule);

    // Remplit les candidats avec les insectes déjà placés sur le plateau
    void remplirCandidatsAvecPlateau(const Joueur* joueur,
                                     const std::map<Hexagon, Insecte*>& plateauSimule);

    // Trouve l'index d'un choix dans les options disponibles
    int findIndexInOptions(const Joueur* joueur, Insecte* insecteChoisi,
                           const std::map<Insecte*, std::vector<Hexagon>>& candidats,
                           const std::vector<Hexagon>& options);

    // Réinitialise les attributs internes de la classe
    void reinitialiserAttributs();

    // Affiche les candidats actuels (debug)
    void afficherCandidats() const;

    // Évalue la cohésion d'un emplacement avec les insectes alliés adjacents
    int evaluerCohesion(const Joueur* joueur, const Hexagon& emplacement,
                        const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue un placement spécifique pour un insecte
    int evaluerPlacementAction(const Joueur* joueur, Insecte* insecte,
                               const Hexagon& emplacement,
                               const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue l'impact d'une attaque sur la reine adverse
    int evaluerAttaqueReineAdverse(const Joueur* joueur, Insecte* insecte,
                                   const Hexagon& nouvelEmplacement,
                                   const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue la protection apportée à la reine alliée
    int evaluerProtectionReine(const Joueur* joueur, Insecte* insecte,
                               const Hexagon& ancienneEmplacement,
                               const Hexagon& nouvelEmplacement,
                               const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue l'impact d'un blocage sur un insecte adverse important
    int evaluerBlocageInsecteImportant(const Joueur* joueur, Insecte* insecte,
                                       const Hexagon& anciennePos,
                                       const Hexagon& nouvelEmplacement,
                                       const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue un déplacement spécifique pour un insecte
    int evaluerDeplacementAction(const Joueur* joueur, Insecte* insecte,
                                 const Hexagon& nouvelEmplacement,
                                 const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue et trie les mouvements possibles pour un insecte
    std::vector<std::pair<Hexagon, int>> evaluerEtTrierMouvements(const Joueur* joueur, Insecte* insecte,
                                                                  const std::vector<Hexagon>& options,
                                                                  bool estPlacement,
                                                                  const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Extrait les meilleurs mouvements parmi les mouvements triés
    std::vector<Hexagon> extraireMeilleursMouvements(const std::vector<std::pair<Hexagon, int>>& mouvementsTries,
                                                     int nombreMax) const;

    // Calcule le score maximum pour un insecte spécifique
    int calculerScoreMaxParInsecte(const Joueur* joueur, Insecte* insecte,
                                   bool estPlacement,
                                   const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Filtre les meilleurs insectes en fonction de leur score
    void filtrerMeilleursInsectes(const Joueur* joueur,
                                  std::map<Insecte*, std::vector<Hexagon>>& insectesCandidats,
                                  int nombreMeilleursInsectes,
                                  bool estPlacement,
                                  const std::map<Hexagon, Insecte*>& plateauSimule);

    // Choisit les heuristiques pour les déplacements
    void choisirHeuristiquePourDeplacer(const Joueur* joueur,
                                        const std::map<Hexagon, Insecte*>& plateauSimule);

    // Choisit les heuristiques pour les placements
    void choisirHeuristiquePourPlacer(const Joueur* joueur,
                                                   const std::vector<Insecte*>& deck,
                                                   const std::map<Hexagon, Insecte*>& plateauSimule);


    // Calcule le score moyen pour les placements
    double calculerScoreMoyenDePlacement(const Joueur* joueur,
                                         const std::map<Insecte*, std::vector<Hexagon>>& candidats,
                                         const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Calcule le score moyen pour les déplacements
    double calculerScoreMoyenDeDeplacement(const Joueur* joueur,
                                           const std::map<Insecte*, std::vector<Hexagon>>& candidats,
                                           const std::map<Hexagon, Insecte*>& plateauSimule) const;


    // Implémentations des fonctions virtuelles
    int getInputForAction() override;
    int getInputForDeckIndex() override;
    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) override;
    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) override;
    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) override;

};

class JoueurIANiveau3 : public JoueurIANiveau2 {
public:
    JoueurIANiveau3(std::string nom, const std::map<Hexagon, Insecte*>* plateauRef, unsigned int* tourRef, Joueur* adversaireRef)
            : JoueurIANiveau2(nom, plateauRef, tourRef), adversaire(adversaireRef) {}

    Joueur* adversaire; // Pointeur vers l'adversaire

    int getInputForAction() override {
        reinitialiserAttributs();

        int meilleurScore = -std::numeric_limits<int>::max();
        std::pair<Insecte*, Hexagon> meilleurCoup{nullptr, Hexagon()};

        // Stacks pour gérer les decks
        std::stack<std::vector<Insecte*>> pileDeckMax;
        std::stack<std::vector<Insecte*>> pileDeckMin;

        // Empiler l'état initial des decks
        pileDeckMax.push(copierDeck(this));
        pileDeckMin.push(copierDeck(adversaire));

        // Récupération des decks courants
        auto& deckMaxCourant = pileDeckMax.top();
        auto& deckMinCourant = pileDeckMin.top();

        auto coupsPossibles = genererCoups(getPlateau(), true, deckMaxCourant, deckMinCourant);

        // Si aucun coup possible, fallback
        if (coupsPossibles.empty()) {
            actionChoisie = AUCUN_ACTION;
            return static_cast<int>(actionChoisie);
        }

        // Parcourir les coups et évaluer via minimax
        for (auto& [insecte, positions] : coupsPossibles) {
            for (auto& position : positions) {
                // Copier les decks actuels avant de simuler le coup
                auto deckMaxCopy = deckMaxCourant;
                auto deckMinCopy = deckMinCourant;

                auto plateauSimule = simulerCoup(getPlateau(), std::make_pair(insecte, position),
                                                 deckMaxCopy, deckMinCopy, true);

                // Empiler les decks mis à jour
                pileDeckMax.push(deckMaxCopy);
                pileDeckMin.push(deckMinCopy);

                int score = minimax(plateauSimule, 3, false, -std::numeric_limits<int>::max(),
                                    std::numeric_limits<int>::max(), pileDeckMax, pileDeckMin);

                // Dépiler après exploration
                pileDeckMax.pop();
                pileDeckMin.pop();

                if (score > meilleurScore) {
                    meilleurScore = score;
                    meilleurCoup = {insecte, position};
                }
            }
        }
        reinitialiserAttributs();
        // Ajouter le meilleur coup aux candidats
        candidats[meilleurCoup.first].push_back(meilleurCoup.second);

        // Déterminer l'action en fonction de si l'insecte était déjà placé ou non
        bool placement = estPlacement(getPlateau(), meilleurCoup.first);
        actionChoisie = placement ? PLACER : DEPLACER;

        mettreAJourCoordonneesInsectes(getPlateau());

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

private:

    int minimax(std::map<Hexagon, Insecte*> plateau, int profondeur, bool maximisateur, int alpha, int beta,
                std::stack<std::vector<Insecte*>>& pileDeckMax, std::stack<std::vector<Insecte*>>& pileDeckMin);

    int evaluerPlateau(const std::map<Hexagon, Insecte*>& plateau,
                       const std::vector<Insecte*>& deckMaximisateur,
                       const std::vector<Insecte*>& deckMinimisateur);

    bool estTerminal(const std::map<Hexagon, Insecte*>& plateau,
                     const std::vector<Insecte*>& deckMaximisateur,
                     const std::vector<Insecte*>& deckMinimisateur);

    std::vector<Insecte*> copierDeck(const Joueur* joueur);

    std::map<Insecte*, std::vector<Hexagon>> genererCoups(const std::map<Hexagon, Insecte*>& plateau, bool estMaximisant,
                                                            const std::vector<Insecte*>& deckMaximisateur,
                                                            const std::vector<Insecte*>& deckMinimisateur);

    std::map<Insecte*, std::vector<Hexagon>> genererCoupsAdversaire(const std::map<Hexagon, Insecte*>& plateau,
                                                                    const std::vector<Insecte*>& deckMaximisateur,
                                                                    const std::vector<Insecte*>& deckMinimisateur);


    std::map<Hexagon, Insecte*> simulerCoup(const std::map<Hexagon, Insecte*>& plateau,
                                            const std::pair<Insecte*, Hexagon>& coup,
                                            std::vector<Insecte*>& deckMaximisateur,
                                            std::vector<Insecte*>& deckMinimisateur,
                                            bool estMaximisant);


    bool estEncerclee(Insecte* reine, const std::map<Hexagon, Insecte*>& plateau) const;

    bool estPlacement(const std::map<Hexagon, Insecte*>& plateau, Insecte* insecte);

    void mettreAJourCoordonneesInsectes(const std::map<Hexagon, Insecte*>& plateau);
};







#endif // JOUEUR_H
