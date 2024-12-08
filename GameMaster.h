#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <fstream>  // Ajout de l'en-tête pour la gestion des fichiers
#include "Joueur.h"
#include "Plateau.h"
#include "Action.h"
#include <iostream>
#include <memory>
#include <limits>
#include <deque>

bool positionEstValide(const Hexagon& position, const std::vector<Hexagon>& deplacementsPossibles);
class GameMaster {
private:
    Plateau plateau;
    Joueur *joueur1;
    Joueur *joueur2;
    unsigned int mode;
    unsigned int modeIA;
    unsigned int tour;
    unsigned int maxInsecte;
    void deplacerPion(Joueur* current);
    void placerPion(Joueur* current, bool b);
    std::deque<Action*> actionsDeque;
    unsigned int maxRetourArriere;
    InsecteFactoryImpl insecteFactory;
    std::string toJson() const;
    void saveGame() const;
    void displaySaveGame();
    void undoLastTwoActions();
public:
    bool hasPlayQueen(Joueur *currentPlayer);
    GameMaster() : joueur1(nullptr), joueur2(nullptr) , mode(0) {}
    Insecte* selectionnerInsecte(Joueur* current);
    GameMaster(Plateau plateau, unsigned int maxRetourArriere = 5)
            : joueur1(nullptr), joueur2(nullptr), mode(0), tour(0), plateau(plateau), maxRetourArriere(maxRetourArriere) {}
    std::vector<Insecte*> creerDeck();
    void startGame();
    void jouer();
    bool detectWinner(Joueur *joueur1 , Joueur *joueur2);


    void saveGame();
    bool verifierProprietairePion(Joueur* current, Insecte* insecte);
    bool verifierDeplacementsPossiblesPourTousLesInsectes(Joueur* current);
    void undoLastAction();
    // Destructeur pour libérer la mémoire
    ~GameMaster();

};



#endif // GAMEMASTER_H
