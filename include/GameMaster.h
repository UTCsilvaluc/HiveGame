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
    void startGame();
    void jouer();
    bool detectWinner(Joueur *joueur1 , Joueur *joueur2);
    void choixExtensions() {
        // Afficher la liste des extensions disponibles
        while (true) {
            std::cout << "\n--- Liste des extensions disponibles ---\n";
            std::vector<std::string> extensionsDisponibles = insecteFactory.getInsecteExtensionsDisponibles();

            if (extensionsDisponibles.empty()) {
                std::cout << "Aucune extension disponible.\n";
                break;
            }

            for (size_t i = 0; i < extensionsDisponibles.size(); ++i) {
                std::string statut = insecteFactory.estExtensionActive(extensionsDisponibles[i]) ? "(Activée)" : "(Non activée)";
                std::cout << i + 1 << ". " << extensionsDisponibles[i] << " " << statut << std::endl;
            }
            std::cout << "\nEntrez le numéro de l'extension à activer/désactiver (0 pour quitter) : ";
            int choix;
            std::cin >> choix;
            if (choix == 0) {
                break;
            }

            if (choix < 1 || choix > extensionsDisponibles.size()) {
                std::cout << "Choix invalide. Veuillez entrer un numéro valide.\n";
                continue;
            }

            std::string extensionChoisie = extensionsDisponibles[choix - 1];
            bool estActive = insecteFactory.estExtensionActive(extensionChoisie);

            if (estActive) {
                std::cout << extensionChoisie << " est déjà activée. Vous voulez la désactiver ? (o/n) : ";
                char reponse;
                std::cin >> reponse;
                if (reponse == 'o' || reponse == 'O') {
                    insecteFactory.desactiverExtension(extensionChoisie);
                    std::cout << extensionChoisie << " a été désactivée.\n";
                }
            } else {
                std::cout << extensionChoisie << " n'est pas encore activée. Vous voulez l'activer ? (o/n) : ";
                char reponse;
                std::cin >> reponse;
                if (reponse == 'o' || reponse == 'O') {
                    insecteFactory.activerExtension(extensionChoisie);
                    std::cout << extensionChoisie << " a été activée.\n";
                }
            }
        }
    }
    void saveGame();
    bool verifierProprietairePion(Joueur* current, Insecte* insecte);
    bool verifierDeplacementsPossiblesPourTousLesInsectes(Joueur* current);
    void undoLastAction();
    // Destructeur pour libérer la mémoire
    ~GameMaster();

};



#endif // GAMEMASTER_H
