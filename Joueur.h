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
#include <functional>    // std::hash
#include <unordered_map> // std::unordered_map
#include <array>

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
    virtual void setAdversaire(Joueur* joueur){}
    void clearDeck(){
        for (Insecte* insecte : deck) {
            delete insecte; // Libère la mémoire occupée par l'objet pointé
        }
        deck.clear(); // Vide le contenu du vector
    }
    void setName(std::string& name){nom = name;}
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

    int getInputForAction() override {
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

    Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour) override{
        std::cout << "Le plateau est vide, vous devez entrer les coordonnées directement.\n";
        int x = getInput("Abscisse pour poser le pion : ", minQ - 1, maxQ + 1 , tour);
        int y = getInput("Ordonnée pour poser le pion : ", minR - 1, maxR + 1 , tour);
        return Hexagon(x,y);
    }

    int getInputForDeckIndex()override{
        return getInput("Quel pion souhaitez-vous poser ? ", 1, getDeckSize()) - 1;
    }

    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) override{
        return getInput("Choisissez un emplacement ou entrez -1 pour annuler : ", -1, placementsPossibles.size());
    }

    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) override{
        return getInput("Entrez l'index de l'insecte à sélectionner (ou -1 pour annuler) : ", -1, insectesDuJoueur.size() - 1);
    }

    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles)override{
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

    int getInputForAction() override{
        return randomChoice();
    }

    Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour) override{
        // À implémenter si on veut faire commencer IA ou faire jouer IA contre IA
        return Hexagon(0,0);
    }

    int getInputForDeckIndex() override{
        return randomDeckChoice();
    }

    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) override{
        return randomHexagonIndexChoice(placementsPossibles);
    }

    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) override{
        return randomPionIndexChoice(insectesDuJoueur);
    }

    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) override{
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

// Clé qui représente l'état local d'une évaluation
// (Insecte*, coordonnée, hash du plateau)
struct HeuristicKey {
    const Insecte* insecte;
    Hexagon coords;
    std::size_t plateauHash; // Un hash global du plateau

    bool operator==(const HeuristicKey& other) const {
        return (insecte == other.insecte && coords == other.coords && plateauHash == other.plateauHash);
    }
};

// Hash custom pour HeuristicKey
struct HeuristicKeyHash {
    std::size_t operator()(const HeuristicKey& key) const {
        // Combine les pointeurs + coords + plateauHash
        // On utilise reinterpret_cast pour le pointeur d’insecte
        auto ptrVal = reinterpret_cast<std::uintptr_t>(key.insecte);
        std::size_t seed = std::hash<std::uintptr_t>()(ptrVal);

        // Combine coords
        seed ^= std::hash<int>()(key.coords.getQ()) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>()(key.coords.getR()) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);

        // Combine plateauHash
        seed ^= std::hash<std::size_t>()(key.plateauHash) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);

        return seed;
    }
};

// Cache global ou membre de classe : stocke le résultat de l’heuristique
static std::unordered_map<HeuristicKey, double, HeuristicKeyHash> heuristicsCache;







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
    std::unordered_map<std::string, double> poidsHeuristiques;

public:
    JoueurIANiveau2(const std::string& nom, const std::map<Hexagon, Insecte*>* plateauRef, unsigned int* tourRef, const std::unordered_map<std::string, double>& initialPoids = {})
        : JoueurIA(nom), plateau(plateauRef), tour(tourRef), poidsHeuristiques(initialPoids) {}

    // Getter pour nouveauxCandidats
    const std::map<Insecte*, std::vector<Hexagon>>& getNouveauxCandidats() const {
        return nouveauxCandidats;
    }

    double getPoids(const std::string& heuristique) const {
        return poidsHeuristiques.at(heuristique); // Directement utiliser at()
    }

    void setPoids(const std::string& heuristique, double poids) {
        poidsHeuristiques[heuristique] = poids; // Modifier ou ajouter la clé directement
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

    void filtrerPlacements(std::vector<Hexagon>& positions,
                                        const Insecte* reineAdverse,
                                        const Insecte* reineAlliee,
                                        const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Réinitialise les attributs internes de la classe
    void reinitialiserAttributs();

    // Affiche les candidats actuels (debug)
    void afficherCandidats() const;

    // Évalue la cohésion d'un emplacement avec les insectes alliés adjacents
    double evaluerCohesion(const Joueur* joueur, Insecte* insecte, const Hexagon& emplacement,
                                        const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue un placement spécifique pour un insecte
    double evaluerPlacementAction(const Joueur* joueur, Insecte* insecte,
                               const Hexagon& emplacement,
                               const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue l'impact d'une attaque sur la reine adverse
    double evaluerAttaqueReineAdverse(const Joueur* joueur, Insecte* insecte,
                                   const Hexagon& nouvelEmplacement,
                                   const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue la protection apportée à la reine alliée
    double evaluerProtectionReine(const Joueur* joueur, Insecte* insecte,
                               const Hexagon& ancienneEmplacement,
                               const Hexagon& nouvelEmplacement,
                               const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue l'impact d'un blocage sur un insecte adverse important
    double evaluerBlocageInsecteImportant(const Joueur* joueur, Insecte* insecte,
                                       const Hexagon& anciennePos,
                                       const Hexagon& nouvelEmplacement,
                                       const std::map<Hexagon, Insecte*>& plateauSimule) const;

    bool choisirCoupAleatoire(const Joueur* joueur,
                                           const std::map<Hexagon, Insecte*>& plateauSimule,
                                           const std::vector<Insecte*>& deck,
                                           bool estPlacement);

    // Évalue un déplacement spécifique pour un insecte
    double evaluerDeplacementAction(const Joueur* joueur, Insecte* insecte,
                                 const Hexagon& nouvelEmplacement,
                                 const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Évalue et trie les mouvements possibles pour un insecte
    std::vector<std::pair<Hexagon, double>> evaluerEtTrierMouvements(const Joueur* joueur, Insecte* insecte,
                                                                  const std::vector<Hexagon>& options,
                                                                  bool estPlacement,
                                                                  const std::map<Hexagon, Insecte*>& plateauSimule) const;

    // Extrait les meilleurs mouvements parmi les mouvements triés
    std::vector<Hexagon> extraireMeilleursMouvements(const std::vector<std::pair<Hexagon, double>>& mouvementsTries,
                                                     int nombreMax) const;

    // Calcule le score maximum pour un insecte spécifique
    double calculerScoreMaxParInsecte(const Joueur* joueur, Insecte* insecte,
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


    double calculerScoreMoyen(const Joueur* joueur,
                                           const std::map<Insecte*, std::vector<Hexagon>>& candidats,
                                           const std::map<Hexagon, Insecte*>& plateauSimule,
                                           bool estPlacement) const;

    void mettreAJourCoordonneesInsectes(const std::map<Hexagon, Insecte*>& plateau) const;

    std::size_t hashPlateau(const std::map<Hexagon, Insecte*>& plateau) const;

    double heuristiquePreliminaire(const Joueur* joueur, Insecte* insecte, const Hexagon& emplacement, const std::map<Hexagon, Insecte*>& plateauSimule) const;



    // Implémentations des fonctions virtuelles
    int getInputForAction() override;
    Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour) override{
        return Hexagon(0,0);
    }
    int getInputForDeckIndex() override;
    int getInputForPlacementIndex(std::vector<Hexagon> placementsPossibles) override;
    int getInputIndexForInsectToMove(std::vector<Insecte*> insectesDuJoueur) override;
    int getInputForMovementIndex(std::vector<Hexagon> deplacementsPossibles) override;

};

struct TranspositionEntry {
    int score;
    int profondeur;
    enum Type { EXACT, LOWER, UPPER } type;
};

// Supposons un nombre maximum de types d'insectes
constexpr int nbTypesInsectes = 9; // Ajustez selon votre jeu (Reine=0, Fourmi=1, etc.)

struct HexagonHash {
    std::size_t operator()(const Hexagon& h) const {
        // Un hash simple sur q et r
        std::size_t seed = 0;
        // Combinaison standard
        seed ^= std::hash<int>()(h.getQ()) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>()(h.getR()) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        return seed;
    }
};

static std::unordered_map<Hexagon, std::array<size_t, nbTypesInsectes>, HexagonHash> zobristKeys;
static std::mt19937_64 rng(std::random_device{}());

size_t generateRandomKeyHash();

// Fonction qui initialise une entrée Zobrist pour une case donnée si elle n'existe pas encore
void ensureZobristKeysForHexHash(const Hexagon& h);

size_t calculerZobristHashPlateau(const std::map<Hexagon, Insecte*>& plateau);

struct GameState {
    size_t zobristHash;
    std::vector<Insecte*> deckMax;
    std::vector<Insecte*> deckMin;
};

struct GameStateHash {
    std::size_t operator()(const GameState& gs) const {
        return gs.zobristHash;
    }
};

struct GameStateEqual {
    bool operator()(const GameState& a, const GameState& b) const {
        return a.zobristHash == b.zobristHash;
    }
};


static std::unordered_map<GameState, TranspositionEntry, GameStateHash, GameStateEqual> transpositionTable;





class JoueurIANiveau3 : public JoueurIANiveau2 {
public:
    JoueurIANiveau3(std::string nom, const std::map<Hexagon, Insecte*>* plateauRef, unsigned int* tourRef, const std::unordered_map<std::string, double>& initialPoids = {}, Joueur* adversaireRef = nullptr)
            : JoueurIANiveau2(nom, plateauRef, tourRef, initialPoids), adversaire(adversaireRef) {}

    Joueur* adversaire; // Pointeur vers l'adversaire

    void setAdversaire(Joueur* adv) {
        adversaire = adv;
    }

    int getInputForAction() override;

    Hexagon getFirstPlacementCoordinates(int minQ, int maxQ, int minR, int maxR, unsigned int tour) override{
        return Hexagon(0,0);
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

    int minimax(std::map<Hexagon, Insecte*> plateau,int profondeur,bool maximisateur,int alpha,int beta,
                                std::vector<Insecte*>& deckMax,std::vector<Insecte*>& deckMin);

    int evaluerPlateau(const std::map<Hexagon, Insecte*>& plateau,
                       const std::vector<Insecte*>& deckMaximisateur,
                       const std::vector<Insecte*>& deckMinimisateur);

    bool estTerminal(const std::map<Hexagon, Insecte*>& plateau,
                     const std::vector<Insecte*>& deckMaximisateur,
                     const std::vector<Insecte*>& deckMinimisateur);

    std::vector<Insecte*> copierDeck(const Joueur* joueur);

    std::map<Insecte*, std::vector<Hexagon>> genererCoups(const std::map<Hexagon, Insecte*>& plateau, bool estMaximisant,
                                                            const std::vector<Insecte*>& deckMaximisateur, const std::vector<Insecte*>& deckMinimisateur);

    std::map<Insecte*, std::vector<Hexagon>> genererCoupsAdversaire(const std::map<Hexagon, Insecte*>& plateau,
                                                                    const std::vector<Insecte*>& deckMaximisateur,
                                                                    const std::vector<Insecte*>& deckMinimisateur);


    std::map<Hexagon, Insecte*> simulerCoup(std::map<Hexagon, Insecte*>& plateau,
                                            const std::pair<Insecte*, Hexagon>& coup,std::vector<Insecte*>& deckMaximisateur,
                                            std::vector<Insecte*>& deckMinimisateur,bool estMaximisant);


    bool estEncerclee(Insecte* reine, const std::map<Hexagon, Insecte*>& plateau) const;

    bool estPlacement(const std::map<Hexagon, Insecte*>& plateau, Insecte* insecte);

    // Fonction pour annuler un coup
    void undoCoup(std::map<Hexagon, Insecte*>& plateau, Insecte* insecte, const Hexagon& anciennePosition, const Hexagon& nouvellePosition,
                bool estPlacement, std::vector<Insecte*>& deckMax, std::vector<Insecte*>& deckMin, bool maximisateur);

};

enum class JoueurType {
    HUMAIN,
    IA,
    IA_NIVEAU2,
    IA_NIVEAU3
};

class JoueurFactory {
private:
    // Exemple de poids par défaut pour IA niveau 2
    static const std::unordered_map<std::string, double>& defaultHeuristicsN2() {
        // On le déclare static pour qu’il soit créé une seule fois
        static std::unordered_map<std::string, double> defaults = {
            {"distanceReineAdverse", 5},
            {"distanceReineAllieeEngorge", 1},
            {"evaluerCohesion", 1},
            {"distanceMin", 10.0},
            {"evaluerAttaqueReineAdverse", 2.0},
            {"potentielFuturPlacement", 0.75},
            {"placement", 0.75},
            {"MenacerReineAdverse", 10.0},
            {"plusMenacerReineAdverse", -5.0},
            {"multiDeRisqueSurReine", 1.5},
            {"bougerReine", 10.0},
            {"bougerInsectePourProtegerReine", 5.0},
            {"laisserReineSecurite", 5.0},
            {"bonusBlocage", 7.0}
        };
        return defaults;
    }



    // Idem pour IA niveau 3, si vous voulez d’autres valeurs
    static const std::unordered_map<std::string, double>& defaultHeuristicsN3() {
        static std::unordered_map<std::string, double> defaults = {
            {"distanceReineAdverse", 5},
            {"distanceReineAllieeEngorge", 1},
            {"evaluerCohesion", 1},
            {"distanceMin", 10.0},
            {"evaluerAttaqueReineAdverse", 2.0},
            {"potentielFuturPlacement", 0.75},
            {"placement", 0.75},
            {"MenacerReineAdverse", 10.0},
            {"plusMenacerReineAdverse", -5.0},
            {"multiDeRisqueSurReine", 1.5},
            {"bougerReine", 10.0},
            {"bougerInsectePourProtegerReine", 5.0},
            {"laisserReineSecurite", 5.0},
            {"bonusBlocage", 7.0}
        };
        return defaults;
    }

public:
    static Joueur* createJoueur(JoueurType type,
                                const std::string& nom,
                                const std::map<Hexagon, Insecte*>* plateau = nullptr,
                                unsigned int* tour = nullptr,
                                Joueur* adversaire = nullptr,
                                const std::unordered_map<std::string, double>& poidsHeuristiques = {})
    {
        switch(type)
        {
        case JoueurType::HUMAIN:
            return new JoueurHumain(nom);

        case JoueurType::IA:
            return new JoueurIA(nom);

        case JoueurType::IA_NIVEAU2:
            if (!plateau || !tour)
                throw std::invalid_argument("[JoueurFactory] Plateau/tour requis pour IA niveau 2.");
            // Si l’appelant ne fournit pas de poids (poidsHeuristiques.empty()),
            // on applique ceux par défaut.
            if (poidsHeuristiques.empty()) {
                return new JoueurIANiveau2(nom, plateau, tour, defaultHeuristicsN2());
            } else {
                return new JoueurIANiveau2(nom, plateau, tour, poidsHeuristiques);
            }

        case JoueurType::IA_NIVEAU3:
            if (!plateau || !tour || !adversaire)
                throw std::invalid_argument("[JoueurFactory] Plateau/tour requis pour IA niveau 3.");
            // Idem pour le niveau 3
            if (poidsHeuristiques.empty()) {
                return new JoueurIANiveau3(nom, plateau, tour, defaultHeuristicsN3(), adversaire);
            } else {
                return new JoueurIANiveau3(nom, plateau, tour, poidsHeuristiques, adversaire);
            }

        default:
            throw std::invalid_argument("[JoueurFactory] Type de joueur inconnu.");
        }
    }
};

#endif // JOUEUR_H
