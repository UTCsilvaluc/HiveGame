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
private:
    ActionType actionChoisie;                           // PLACER, DEPLACER, AUCUN
    Hexagon positionChoisie;                            // Pour mémoriser la position choisie
    Insecte* insecteChoisi;                             // Pour mémoriser l'insecte choisi (peut être dans le deck ou sur le plateau)
    std::map<Insecte*, std::vector<Hexagon>> nouveauxCandidats; // Nouveaux candidats pour chaque heuristique
    std::map<Insecte*, std::vector<Hexagon>> candidats; // Map pour stocker les insectes et leurs déplacements possibles
    std::vector<HeuristiqueType> historiqueHeuristiques; // Historique des heuristiques choisies
    const std::map<Hexagon, Insecte*>* plateau;         // Pointeur vers le plateau
    unsigned int* tour;                                 // Pointeur vers le numéro du tour

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
    int getActionPourGameMaster() const { return static_cast<int>(actionChoisie); }
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
    void afficherHistoriqueHeuristiques() const;
    void afficherCandidats() const;
    void remplirCandidatsAvecDeck();
    void filtrerPrioriteFourmies();
    int evaluerCohesion(const Hexagon& emplacement) const;
    int evaluerDeplacementsApresPlacement(const Hexagon& emplacement, Insecte* insecte) const;
    void filtrerMeilleursPlacementsPourDeplacements(int nombreMaxPlacements);

    void choisirAction();

    void intersectionCandidats();
    void protegerReine();
    void attaquerReine();
    void verifierDeplacementsReine(Insecte* reine, const std::vector<Hexagon>& ennemisVoisins);
    void verifierDeplacementsAllies(Insecte* reine, const std::vector<Hexagon>& voisinsReine);
    int evaluerAction(const Hexagon& emplacement, Insecte* insecte, const std::map<Hexagon, Insecte*>& plateau) const;
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




#endif // JOUEUR_H
