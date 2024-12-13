#include "GameMaster.h"
void GameMaster::startGame() {
    //displaySaveGame();
    std::unordered_map<std::string, double> poidsIA = {
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

    std::ifstream file("game_save82.json");
    std::cout << "\nD�marrage du jeu HiveGame en cours...\n" << std::endl;
    choixExtensions();
    mode = getInput("Merci de s�lectionner le mode de jeu :\n1 - Joueur vs Joueur (JvJ)\n2 - Joueur vs IA (JvIA)\n", 1, 2);
    //std::cout << "Vous avez s�lectionn� le mode : " << (mode == 1 ? "JvJ" : "JvIA") << "\n";
    if (mode == 1 ) std::cout << "Vous avez s�lectionn� le mode : JvJ" << "\n";
    else{
        modeIA = getInput("\nMerci de sï¿½lectionner le niveau de l'IA :\n1 - Niveau 1 (IA Aleatoire)\n2 - Niveau 2 (IA Heuristique)\n3 - Niveau 3 (IA Niveau MAX)\n", 1, 3);
        std::cout << "Vous avez sï¿½lectionnï¿½ le niveau : " << (modeIA == 1 ? "Niveau 1" : "Niveau 2") << "\n";
    }


    std::string nom;
    std::cout << "\nMerci de saisir le nom du Joueur" << std::endl;
    std::cin >> nom;

    joueur1 = new JoueurHumain(nom);  // Cr�er le joueur 1

    if (mode == 1) {
        std::cout << "\nMerci de saisir le nom du second Joueur" << std::endl;
        std::cin >> nom;
        joueur2 = new JoueurHumain(nom);  // Crï¿½er le joueur 2
    } else if (modeIA == 1) {
        joueur2 = new JoueurIA("IA");
    }else if (modeIA == 2){
        joueur2 = new JoueurIANiveau2("IA", &plateau.getPlateauMap(), &tour, poidsIA);
    }else if(modeIA == 3){
        joueur2 = new JoueurIANiveau3("IA", &plateau.getPlateauMap(), &tour, poidsIA, joueur1);
    }


    std::cout << "Joueur 1 cr�� : " << joueur1->getName() << std::endl;
    std::cout << "Joueur 2 cr�� : " << joueur2->getName() << std::endl;
    jouer();
}

void GameMaster::startGameForIA() {
    std::unordered_map<std::string, double> poidsIA1 = {
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

    std::unordered_map<std::string, double> poidsIA2 = {
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

    joueur1 = new JoueurIANiveau2("IA1", &plateau.getPlateauMap(), &tour, poidsIA1);  // Cr�er le joueur 1
    joueur2 = new JoueurIANiveau2("IA2", &plateau.getPlateauMap(), &tour, poidsIA2);
    jouer();
}

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "GameMaster.h"
#include "Insecte.h"  // Supposons que tu as une classe Insecte avec les coordonnées q, r

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "GameMaster.h"
#include "Insecte.h"  // Supposons que tu as une classe Insecte avec les coordonnées q, r

void GameMaster::displaySaveGame() {
    // Ouvre le fichier save_game.json
    std::ifstream file("game_save2.json");

    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier game_save2.json" << std::endl;
        return;
    }

    std::stringstream jsonData;
    jsonData << file.rdbuf();  // Lire tout le contenu du fichier dans un stringstream
    file.close();  // Fermer le fichier après la lecture

    std::string jsonContent = jsonData.str();  // Contenu du fichier sous forme de chaîne de caractères

    std::cout << "Affichage des insectes dans le fichier game_save2.json..." << std::endl;

    // Exemple d'extraction et d'affichage de données des insectes
    size_t pos = 0;
    std::string insectPattern = "\"nom\": \"";
    while ((pos = jsonContent.find(insectPattern, pos)) != std::string::npos) {
        size_t startNom = jsonContent.find("\"nom\": \"", pos) + 8;  // Début du nom
        size_t endNom = jsonContent.find("\"", startNom);  // Fin du nom
        std::string nomInsecte = jsonContent.substr(startNom, endNom - startNom);  // Extraire le nom

        // Extraire les coordonnées
        size_t startQ = jsonContent.find("\"q\":", endNom) + 4;
        size_t endQ = jsonContent.find(",", startQ);
        int q = std::stoi(jsonContent.substr(startQ, endQ - startQ));  // Extraire q

        size_t startR = jsonContent.find("\"r\":", endQ) + 4;
        size_t endR = jsonContent.find("}", startR);
        int r = std::stoi(jsonContent.substr(startR, endR - startR));  // Extraire r

        // Extraire le nom du propriétaire
        size_t startOwner = jsonContent.find("\"owner\": \"", endR) + 10;
        size_t endOwner = jsonContent.find("\"", startOwner);
        std::string owner = jsonContent.substr(startOwner, endOwner - startOwner);  // Extraire le propriétaire

        // Afficher les informations de l'insecte
        std::cout << "Insecte : " << nomInsecte << ", Coordonnées (q, r) : (" << q << ", " << r << "), Propriétaire : " << owner << std::endl;

        // Avance la position dans le fichier JSON
        pos = endR;
    }
}


GameMaster::~GameMaster() {
    delete joueur1;
    delete joueur2;

    // Lib�rer la m�moire de la pile d'actions
    while (!actionsDeque.empty()) {
        delete actionsDeque.back();
        actionsDeque.pop_back();
    }
}

bool positionEstValide(const Hexagon& position, const std::vector<Hexagon>& deplacementsPossibles) {
    return std::find(deplacementsPossibles.begin(), deplacementsPossibles.end(), position) != deplacementsPossibles.end();
}

void GameMaster::deplacerPion(Joueur* current) {
    plateau.afficherPlateau(joueur1, joueur2);

    Insecte* currentInsecte = selectionnerInsecte(current);
    if (currentInsecte == nullptr){
        tour--;
        return;
    }

    bool deplacementValide = false;
    Hexagon nouvellePosition;

    int x = 0;
    int y = 0;

    while (!deplacementValide) {
        std::vector<Hexagon> deplacementsPossibles = currentInsecte->deplacementsPossibles(plateau.getPlateauMap());
        plateau.afficherPlateauAvecPossibilites(deplacementsPossibles, joueur1, joueur2, current);
        plateau.afficherPossibilitesDeplacements(currentInsecte, deplacementsPossibles);

        int choix = current->getInputForMovementIndex(deplacementsPossibles);

        // Annuler si l'utilisateur entre -1
        if (choix == -1) {
            std::cout << "Déplacement annulé.\n";
            tour--;
            return;
        }

        Hexagon position = deplacementsPossibles[choix - 1];
        x = position.getQ();
        y = position.getR();

        nouvellePosition = Hexagon(x, y);
        if (positionEstValide(nouvellePosition, deplacementsPossibles)) {
            deplacementValide = true;
        } else {
            std::cout << "Déplacement invalide. Veuillez choisir une coordonnée valide." << std::endl;
        }
    }



    Action* action = currentInsecte->actionDeplacer(nouvellePosition);
    action->executerAction(plateau); // `plateau` est l'instance de Plateau utilisée dans votre jeu;
    if (actionsDeque.size() >= maxRetourArriere) {
        Action* actionToDelete = actionsDeque.front();
        delete actionToDelete;
        actionsDeque.pop_front();
    }
    actionsDeque.push_back(action);
}


void GameMaster::jouer() {
    bool hasWinner = false;
    while (!hasWinner) {
        Joueur* current = (tour % 2 == 0) ? joueur1 : joueur2;
        int playerTurn = (tour / 2) + 1; // Tour sp�cifique au joueur
        hasWinner = detectWinner(joueur1, joueur2);

        std::cout << "\nC'est au tour de : " << current->getName() << std::endl;

        if (current->hasQueen()) {
            int remainingTurnsToPlayQueen = 4 - playerTurn;
            std::cout << "Il vous reste " << remainingTurnsToPlayQueen
                      << " tours pour jouer votre reine." << std::endl;
        }


        int choice = 0;
        bool needPlayQueen = (playerTurn >= 4) && current->hasQueen();

        if (needPlayQueen) {
            std::cout << "Vous devez obligatoirement poser votre Reine !\n";
            choice = 2; // Forcer le choix de poser la reine
        } else {
            if (!plateau.playerCanMoveInsecte(current)) {
                std::cout << "Aucun mouvement possible, vous devez placer un pion.\n";
                choice = 2;
            } else {
                choice = current->getInputForAction();
            }
        }
        while (true) {
            if (choice == 3) {
                if (actionsDeque.size() >= 2) {
                    undoLastTwoActions();
                    break;
                } else {
                    std::cout << "Aucune action � annuler, essayez une autre option.\n";
                    choice = current->getInputForAction();
                    continue;
                }
            } else if (choice == 1) {
                deplacerPion(current);
                plateau.afficherPlateau(joueur1, joueur2);
                break;
            } else if (choice == 2) {  // Placer un nouveau pion
                placerPion(current, needPlayQueen);
                plateau.afficherPlateau(joueur1, joueur2);
                break;
            } else {
                std::cout << "Choix invalide. Veuillez r�essayer.\n";
                choice = current->getInputForAction();
            }
        }
        tour++;
        //saveGame();
    }
}

void GameMaster::placerPion(Joueur* current, bool needPlayQueen) {
    Insecte* insecteAPlacer = nullptr;
    int index = 0;

    // S�lection du pion � placer
    while (insecteAPlacer == nullptr){
        if (current->getDeckSize()==0) break;
        if (!needPlayQueen) {
            std::cout << "\nVoici votre deck : " << std::endl;
            current->afficherDeck();
            index = current->getInputForDeckIndex();
            insecteAPlacer = current->getInsecteByIndex(index);
        } else {
            insecteAPlacer = current->getQueen();
            index = current->getQueenIndex();
        }
    }

    Hexagon position;
    bool placementValide = false;

    // Boucle pour garantir un placement valide
    while (!placementValide) {
        if (plateau.plateauEstVide()) {
            position = current->getFirstPlacementCoordinates(plateau.getMinQ(), plateau.getMaxQ(), plateau.getMinR(), plateau.getMaxR(), tour);
            placementValide = true;
        }
        else{
            std::vector<Hexagon> placementsPossibles = plateau.getPlacementsPossibles(insecteAPlacer);

            plateau.afficherPlateauAvecPossibilites(placementsPossibles, joueur1, joueur2, current);
            plateau.afficherPossibilitesPlacements(insecteAPlacer, placementsPossibles);

            int choix = current->getInputForPlacementIndex(placementsPossibles);

            if (choix == -1) {
                std::cout << "Placement annulé.\n";
                tour--;
                return;
            }
            position = placementsPossibles[choix - 1];

            if (std::find(placementsPossibles.begin(), placementsPossibles.end(), position) != placementsPossibles.end() || plateau.plateauEstVide()){
                placementValide = true;
            }
        }

    }
    Action* action = insecteAPlacer->actionPlacer(position);
    action->executerAction(plateau); // `plateau` est l'instance de Plateau utilisée dans votre jeu
    current->retirerInsecte(index);
    Insecte* insecteEnDessous = plateau.getInsecteAt(position);
    if (actionsDeque.size() >= maxRetourArriere) {
        Action* actionToDelete = actionsDeque.front();
        delete actionToDelete;
        actionsDeque.pop_front();
    }
    actionsDeque.push_back(action);
}


bool GameMaster::detectWinner(Joueur *joueur1 , Joueur *joueur2) {
    Insecte* reineP1 = plateau.getReineAbeille(joueur1);
    Insecte* reineP2 = plateau.getReineAbeille(joueur2);
    if (reineP1 && plateau.estEntouree(reineP1)) {
        std::cout << "Le joueur " << joueur1->getName() << " a perdu ! " << std::endl;
        return true;
    }
    if (reineP2 && plateau.estEntouree(reineP2)) {
        std::cout << "Le joueur " << joueur1->getName() << " a perdu ! " << std::endl;
        return true;
    }
    return false;
}

Insecte* GameMaster::selectionnerInsecte(Joueur* current) {
    std::vector<Insecte*> insectesDuJoueur = current->getInsectesDuJoueur(plateau.getPlateauMap());
    if (insectesDuJoueur.empty()) {
        std::cout << "Aucun insecte du joueur sur le plateau." << std::endl;
        return nullptr;
    }

    std::vector<Hexagon> deplacementsPossiblesPion;
    int indexChoisi = -1;

    do {
        std::cout << "Choisissez un insecte par son index :\n";
        for (size_t i = 0; i < insectesDuJoueur.size(); ++i) {
            std::cout << i << ": " << insectesDuJoueur[i]->getNom() << " - Position: ("
                      << insectesDuJoueur[i]->getCoords().getQ() << ", "
                      << insectesDuJoueur[i]->getCoords().getR() << ")\n";
        }

        indexChoisi = current->getInputIndexForInsectToMove(insectesDuJoueur);
        if (indexChoisi == -1) {
            std::cout << "Action annulée.\n";
            return nullptr;
        }

        deplacementsPossiblesPion = insectesDuJoueur[indexChoisi]->deplacementsPossibles(plateau.getPlateauMap());

        if (deplacementsPossiblesPion.empty()) {
            std::cout << "Cet insecte n'a pas de déplacements possibles. Veuillez en choisir un autre.\n";
        }

    } while (deplacementsPossiblesPion.empty());

    return insectesDuJoueur[indexChoisi];
}



void GameMaster::undoLastAction() {
    if (actionsDeque.size() < 2) {
        std::cout << "Aucune action � annuler, la pile est vide.\n";
        return;  // Aucune action � annuler
    }

    // R�cup�rer la derni�re action de la pile
    Action* lastAction = actionsDeque.back();
    if (lastAction) {
        lastAction->undo(plateau);
    } else {
        std::cerr << "lastAction is nullptr!" << std::endl;
    }

    actionsDeque.pop_back();
    delete lastAction;
    std::cout << "Derni�re action annul�e.\n";
    tour--;
}

void GameMaster::undoLastTwoActions() {
    if (actionsDeque.size() < 2) {
        std::cout << "Pas assez d'actions dans la pile pour annuler. Minimum requis : 2.\n";
        return;  // Pas assez d'actions à annuler
    }

    for (int i = 0; i < 2; ++i) {
        // Récupérer la dernière action de la pile
        Action* lastAction = actionsDeque.back();
        if (lastAction) {
            lastAction->undo(plateau);
        } else {
            std::cerr << "lastAction is nullptr!" << std::endl;
        }
        actionsDeque.pop_back();
        delete lastAction;
        std::cout << "Action annulée (" << (i + 1) << "/2).\n";
        tour--;  // Réduire le numéro du tour
    }
    tour--;
}

bool GameMaster::verifierProprietairePion(Joueur* current, Insecte* insecte) {
    if (insecte && insecte->getOwner() == current) {
        return true;
    }
    std::cout << "Ce pion ne vous appartient pas. Veuillez s�lectionner un de vos pions.\n";
    return false;
}

bool GameMaster::verifierDeplacementsPossiblesPourTousLesInsectes(Joueur* current) {
    for (Insecte* insecte : current->getDeck()) {
        std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateau.getPlateauMap());
        if (!deplacementsPossibles.empty()) {
            return true;
        }
    }
    return false;
}


#include <fstream>
#include <sstream>

std::string GameMaster::toJson() const {
    std::stringstream jsonData;
    jsonData << "{\n";

    // Plateau
    jsonData << "  \"plateau\": " << plateau.toJson() << ",\n";

    // Joueur 1
    jsonData << "  \"joueur1\": "
             << (joueur1 ? joueur1->toJson() : "null") << ",\n";

    // Joueur 2
    jsonData << "  \"joueur2\": "
             << (joueur2 ? joueur2->toJson() : "null") << ",\n";

    // Mode
    jsonData << "  \"mode\": " << mode << ",\n";

    // Tour
    jsonData << "  \"tour\": " << tour << ",\n";

    // Max retours arrière
    jsonData << "  \"maxRetourArriere\": " << maxRetourArriere << "\n";

    jsonData << "}";
    return jsonData.str();
}

#include <fstream>
#include <sstream>

bool fileExists(const std::string& fileName) {
    std::ifstream file(fileName);
    return file.good();
}

void GameMaster::saveGame() {
    std::string baseFileName = "game_save";
    std::string fileExtension = ".json";
    std::string fileName = baseFileName + fileExtension;

    // Vérifier si un fichier existe déjà
    int fileIndex = 0;
    while (fileExists(fileName)) {
        ++fileIndex;
        std::ostringstream oss;
        oss << baseFileName << fileIndex << fileExtension;
        fileName = oss.str();
    }

    // Écrire dans le fichier disponible
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << toJson();
        file.close();
        std::cout << "Partie sauvegardée dans le fichier : " << fileName << std::endl;
    } else {
        std::cerr << "Erreur : Impossible de sauvegarder la partie." << std::endl;
    }
}



#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
/*
void GameMaster::loadGame(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Erreur : Impossible d'ouvrir le fichier de sauvegarde.");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();

    // Simple parsing pour extraire les parties de données du JSON
    // Vous devrez probablement ajuster cela pour que ce soit plus robuste

    // Exemple de découpe basique du JSON
    if (jsonContent.find("\"plateau\"") != std::string::npos) {
        // Extraire et reconstruire l'objet Plateau
        size_t startPos = jsonContent.find("\"plateau\":") + 10;
        size_t endPos = jsonContent.find("\"joueur1\":");
        std::string plateauData = jsonContent.substr(startPos, endPos - startPos);

        // Appel de la méthode fromJson pour Plateau
        plateau.fromJson(plateauData); // Implémentez fromJson pour Plateau

        // Extraire et reconstruire les joueurs
        startPos = endPos + 10;
        endPos = jsonContent.find("\"mode\":");
        std::string joueur1Data = jsonContent.substr(startPos, endPos - startPos);

        joueur1->fromJson(joueur1Data); // Implémentez fromJson pour Joueur

        // Idem pour le joueur2 et autres paramètres
        startPos = endPos + 7;
        endPos = jsonContent.find("\"maxRetourArriere\":");
        std::string joueur2Data = jsonContent.substr(startPos, endPos - startPos);

        joueur2->fromJson(joueur2Data);
        mode = std::stoi(jsonContent.substr(endPos + 19, 1)); // Extraction de mode
        tour = std::stoi(jsonContent.substr(endPos + 27, 1)); // Extraction de tour
        maxRetourArriere = std::stoi(jsonContent.substr(endPos + 41, 1)); // Extraction de maxRetourArriere
    }

    file.close();
    std::cout << "Partie chargée avec succès depuis " << fileName << "." << std::endl;
}
*/



