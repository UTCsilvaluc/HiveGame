#include <dirent.h> // Pour ouvrir et lire les fichiers dans un répertoire
#include <iostream>
#include "Insecte.h"  // Supposons que tu as une classe Insecte avec les coordonnées q, r
#include <regex>
#include <fstream>
#include <sstream>
#include <string>
#include "GameMaster.h"
using namespace std;

#include <unordered_map>
#include <vector>
void GameMaster::startGame() {
    int choix;
    std::cout << "Bienvenue dans HiveGame !\n";
    std::cout << "Que souhaitez-vous faire ?\n";
    std::cout << "1 - Lancer une nouvelle partie\n";
    std::cout << "2 - Charger une partie existante\n";
    std::cout << "Veuillez entrer votre choix (1 ou 2) : ";
    std::cin >> choix;

    // Sécurisation de l'entrée
    while (choix != 1 && choix != 2) {
        std::cout << "Choix invalide. Veuillez entrer 1 pour une nouvelle partie ou 2 pour charger une partie existante : ";
        std::cin >> choix;
    }
    if (choix == 1) {
        // Lancer une nouvelle partie
        std::unordered_map<std::string, double> poidsIA = {
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
        std::cout << "\nDémarrage du jeu HiveGame en cours...\n" << std::endl;
        mode = getInput("Merci de sélectionner le mode de jeu :\n1 - Joueur vs Joueur (JvJ)\n2 - Joueur vs IA (JvIA)\n", 1, 2);
        if (mode == 1) std::cout << "Vous avez sélectionné le mode : JvJ\n";
        else {
            modeIA = getInput("\nMerci de sélectionner le niveau de l'IA :\n1 - Niveau 1 (IA Aléatoire)\n2 - Niveau 2 (IA Heuristique)\n3 - Niveau 3 (IA Niveau MAX)\n", 1, 3);
            std::cout << "Vous avez sélectionné le niveau : " << (modeIA == 1 ? "Niveau 1" : (modeIA == 2 ? "Niveau 2" : "Niveau 3")) << "\n";
        }

        std::string nom;
        std::cout << "\nMerci de saisir le nom du Joueur : ";
        std::cin >> nom;

        joueur1 = new JoueurHumain(nom);  // Créer le joueur 1

        if (mode == 1) {
            std::cout << "\nMerci de saisir le nom du second Joueur : ";
            std::cin >> nom;
            joueur2 = new JoueurHumain(nom);  // Créer le joueur 2
        } else {
            joueur2 = (modeIA == 1) ? new JoueurIA("IA") :
                      (modeIA == 2) ? new JoueurIANiveau2("IA", &plateau.getPlateauMap(), &tour, poidsIA) :
                      (modeIA == 3) ? new JoueurIANiveau3("IA", &plateau.getPlateauMap(), &tour, poidsIA, joueur1) : nullptr;
        }
        creerDeckPourJoueurs();
    } else {
        // Charger une partie existante
        std::cout << "Chargement d'une partie existante...\n";
        std::vector<std::string> fichiersParties;
        DIR* dir = opendir("./"); // Ouvre le répertoire courant
        struct dirent* entry;

        if (dir == nullptr) {
            std::cout << "Erreur d'ouverture du répertoire.\n";
            return;
        }

        // Liste tous les fichiers qui commencent par "game_save"
        while ((entry = readdir(dir)) != nullptr) {
            if (std::string(entry->d_name).find("game_save") == 0) {
                fichiersParties.push_back(entry->d_name);
            }
        }
        closedir(dir); // Ferme le répertoire

        if (fichiersParties.empty()) {
            std::cout << "Aucune sauvegarde trouvée.\n";
            return;
        }

        std::cout << "Voici les sauvegardes disponibles :\n";
        for (size_t i = 0; i < fichiersParties.size(); ++i) {
            std::cout << i + 1 << " - " << fichiersParties[i] << "\n";
        }

        int choixFichier;
        std::cout << "Veuillez entrer le numéro de la sauvegarde à charger : ";
        std::cin >> choixFichier;

        // Sécurisation de l'entrée
        while (choixFichier < 1 || choixFichier > fichiersParties.size()) {
            std::cout << "Choix invalide. Veuillez entrer un numéro entre 1 et " << fichiersParties.size() << " : ";
            std::cin >> choixFichier;
        }

        // Charger la partie choisie
        std::string fichierCharge = fichiersParties[choixFichier - 1];
        std::map<Hexagon, Insecte*> newPlat = afficherFichierAvecBlocs(fichierCharge);
        if (newPlat.size() > 0) {
            plateau.setPlateauMap(newPlat);
        } else {
            std::cout << "Erreur lors du chargement de la partie.\n";
        }
    }

    jouer();
}
void GameMaster::creerDeckPourJoueurs() {
    InsecteFactoryImpl factory(maxInsecte);
    std::vector<Insecte*> deck = configurerDeck(factory);
    joueur1->clearDeck();
    joueur2->clearDeck();
    std::cout << "reussite2";
    for (size_t i = 0; i < deck.size(); ++i) {
        Insecte* insecte = deck[i];
        Insecte* newInsecte1 = factory.createInsecte(insecte->getNom() , Hexagon(0,0) , joueur1);
        Insecte* newInsecte2 = factory.createInsecte(insecte->getNom() , Hexagon(0,0) , joueur2);
        joueur1->ajouterInsecte(newInsecte1);
        joueur2->ajouterInsecte(newInsecte2);
    }
}

// Fonction auxiliaire pour configurer le deck commun
std::vector<Insecte*> GameMaster::configurerDeck(InsecteFactoryImpl& factory) {
    while (true) {
        std::cout << "\n--- Configuration du deck commun ---\n";
        std::cout << "Extensions activées :\n";
        auto extensionsActives = factory.getInsecteExtensionsActives();
        for (size_t i = 0; i < extensionsActives.size(); ++i) {
            std::cout << "(" << i + 1 << ") - " << extensionsActives[i] << "\n";
        }

        std::cout << "\nOptions :\n";
        std::cout << "1. Activer une extension\n";
        std::cout << "2. Désactiver une extension\n";
        std::cout << "3. Modifier le deck (ajouter/retirer occurrences)\n";
        std::cout << "4. Générer le deck\n";
        std::cout << "Votre choix : ";

        int choix;
        std::cin >> choix;

        if (choix == 1) {
            // Activer une extension
            auto extensionsDisponibles = factory.getInsecteExtensionsDisponibles();
            if (extensionsDisponibles.empty()) {
                std::cout << "Aucune extension disponible.\n";
                continue;
            }
            std::cout << "Extensions disponibles :\n";
            for (size_t i = 0; i < extensionsDisponibles.size(); ++i) {
                std::cout << "(" << i + 1 << ") - " << extensionsDisponibles[i] << "\n";
            }
            std::cout << "Entrez l'index de l'extension à activer : ";
            int index;
            std::cin >> index;
            if (index > 0 && index <= static_cast<int>(extensionsDisponibles.size())) {
                factory.activerExtension(extensionsDisponibles[index - 1]);
            } else {
                std::cout << "Index invalide.\n";
            }

        } else if (choix == 2) {
            // Désactiver une extension
            if (extensionsActives.empty()) {
                std::cout << "Aucune extension activée.\n";
                continue;
            }
            std::cout << "Extensions activées :\n";
            for (size_t i = 0; i < extensionsActives.size(); ++i) {
                std::cout << "(" << i + 1 << ") - " << extensionsActives[i] << "\n";
            }
            std::cout << "Entrez l'index de l'extension à désactiver : ";
            int index;
            std::cin >> index;
            if (index > 0 && index <= static_cast<int>(extensionsActives.size())) {
                factory.desactiverExtension(extensionsActives[index - 1]);
            } else {
                std::cout << "Index invalide.\n";
            }

        } else if (choix == 3) {
            // Modifier le deck
            factory.afficherInsectes();
            std::cout << "1. Ajouter occurrence\n2. Retirer occurrence\n";
            int action;
            std::cin >> action;

            std::cout << "Liste des insectes disponibles :\n";
            auto insectesDisponibles = factory.getInsecteExtensionsDisponibles();
            for (size_t i = 0; i < insectesDisponibles.size(); ++i) {
                std::cout << "(" << i + 1 << ") - " << insectesDisponibles[i] << "\n";
            }
            std::cout << "Entrez l'index de l'insecte : ";
            int index;
            std::cin >> index;

            if (index > 0 && index <= static_cast<int>(insectesDisponibles.size())) {
                std::string nomInsecte = insectesDisponibles[index - 1];
                if (action == 1) {
                    factory.ajouterOccurrence(nomInsecte);
                } else if (action == 2) {
                    factory.retirerOccurrence(nomInsecte);
                } else {
                    std::cout << "Action invalide.\n";
                }
            } else {
                std::cout << "Index invalide.\n";
            }

        } else if (choix == 4) {
            return factory.genererDeck(Hexagon(0, 0), nullptr); // null comme placeholder pour le joueur
        } else {
            std::cout << "Choix invalide. Veuillez réessayer.\n";
        }
    }
}

std::vector<Insecte*> GameMaster::creerDeck() {
    InsecteFactoryImpl factory(13);

    while (true) {
        std::cout << "\n--- Configuration des extensions et du deck ---\n";
        std::cout << "Extensions activées :\n";
        auto extensionsActives = factory.getInsecteExtensionsActives();
        for (size_t i = 0; i < extensionsActives.size(); ++i) {
            std::cout << "(" << i + 1 << ") - " << extensionsActives[i] << "\n";
        }

        std::cout << "\nOptions :\n";
        std::cout << "1. Activer une extension\n";
        std::cout << "2. Désactiver une extension\n";
        std::cout << "3. Modifier le deck (ajouter/retirer occurrences)\n";
        std::cout << "4. Générer le deck\n";
        std::cout << "Votre choix : ";

        int choix;
        std::cin >> choix;

        if (choix == 1) {
            // Activer une extension
            auto extensionsDisponibles = factory.getInsecteExtensionsDisponibles();
            if (extensionsDisponibles.empty()) {
                std::cout << "Aucune extension disponible.\n";
                continue;
            }
            std::cout << "Extensions disponibles :\n";
            for (size_t i = 0; i < extensionsDisponibles.size(); ++i) {
                std::cout << "(" << i + 1 << ") - " << extensionsDisponibles[i] << "\n";
            }
            std::cout << "Entrez l'index de l'extension à activer : ";
            int index;
            std::cin >> index;
            if (index > 0 && index <= static_cast<int>(extensionsDisponibles.size())) {
                factory.activerExtension(extensionsDisponibles[index - 1]);
            } else {
                std::cout << "Index invalide.\n";
            }

        } else if (choix == 2) {
            // Désactiver une extension
            if (extensionsActives.empty()) {
                std::cout << "Aucune extension activée.\n";
                continue;
            }
            std::cout << "Extensions activées :\n";
            for (size_t i = 0; i < extensionsActives.size(); ++i) {
                std::cout << "(" << i + 1 << ") - " << extensionsActives[i] << "\n";
            }
            std::cout << "Entrez l'index de l'extension à désactiver : ";
            int index;
            std::cin >> index;
            if (index > 0 && index <= static_cast<int>(extensionsActives.size())) {
                factory.desactiverExtension(extensionsActives[index - 1]);
            } else {
                std::cout << "Index invalide.\n";
            }

        } else if (choix == 3) {
            // Modifier le deck
            factory.afficherInsectes();
            std::cout << "1. Ajouter occurrence\n2. Retirer occurrence\n";
            int action;
            std::cin >> action;

            std::cout << "Liste des insectes disponibles :\n";
            auto insectesDisponibles = factory.getInsecteExtensionsDisponibles();  // Récupérer les insectes jouables
            for (size_t i = 0; i < insectesDisponibles.size(); ++i) {
                std::cout << "(" << i + 1 << ") - " << insectesDisponibles[i] << "\n";
            }
            std::cout << "Entrez l'index de l'insecte : ";
            int index;
            std::cin >> index;

            if (index > 0 && index <= static_cast<int>(insectesDisponibles.size())) {
                std::string nomInsecte = insectesDisponibles[index - 1];
                if (action == 1) {
                    factory.ajouterOccurrence(nomInsecte);
                } else if (action == 2) {
                    factory.retirerOccurrence(nomInsecte);
                } else {
                    std::cout << "Action invalide.\n";
                }
            } else {
                std::cout << "Index invalide.\n";
            }

        } else if (choix == 4) {
            // Générer le deck
            return factory.genererDeck(Hexagon(0, 0), joueur1);
        } else {
            std::cout << "Choix invalide. Veuillez réessayer.\n";
        }
    }
}


void GameMaster::startGameForIA() {
    std::unordered_map<std::string, double> poidsIA1 = {
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

    std::unordered_map<std::string, double> poidsIA2 = {
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

    joueur1 = new JoueurIANiveau2("IA1", &plateau.getPlateauMap(), &tour, poidsIA1);  // Cr�er le joueur 1
    joueur2 = new JoueurIANiveau2("IA2", &plateau.getPlateauMap(), &tour, poidsIA2);
    //joueur1->setAdversaire(joueur2);
    jouer();
}



std::map<Hexagon, Insecte*> GameMaster::afficherFichierAvecBlocs(const std::string& cheminFichier) {
    InsecteFactoryImpl factory;
    std::map<Hexagon, Insecte*> pMap;
    std::map<std::string, Insecte*> mapID_Insecte;
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
    // Mots-clés pour détecter le début des blocs
    std::vector<std::string> debutBlocs = {"joueur1", "joueur2" , "insectesSurPlateau"};
    std::map<std::string, Joueur*> joueurs;
    std::ifstream fichier(cheminFichier); // Ouvre le fichier
    if (!fichier.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << cheminFichier << "\n";

    }

    std::string ligne;
    std::map<std::string, std::string> listeContent; // Contenu des blocs
    std::string contenuBloc = "";  // Accumulateur pour le contenu du bloc
    int niveauAccolade = 0; // Suivi de l'imbrication des accolades
    std::string blocEnCours = "";

    // Variables pour afficher les valeurs spécifiques
    int mode = -1;
    unsigned int tour = -1;
    int maxRetourArriere = -1;
    int modeIA = -1;

    // Lecture ligne par ligne
    while (std::getline(fichier, ligne)) {
        // Recherche des blocs
        for (const std::string& bloc : debutBlocs) {
            if (ligne.find(bloc) != std::string::npos) {
                std::cout<< bloc << " trouvé !";
                blocEnCours = bloc; // Définit le bloc en cours
                contenuBloc = "";    // Réinitialise le contenu du bloc
                niveauAccolade = 0;  // Réinitialise l'imbrication
            }
        }

        // Si nous sommes dans un bloc, accumuler le contenu
        if (!blocEnCours.empty()) {
            contenuBloc += ligne + "\n";
        }

        // Compte les accolades ouvrantes et fermantes pour délimiter les blocs : même algo que les opérations arithmétiques
        for (char c : ligne) {
            if (c == '{') {
                niveauAccolade++;
            } else if (c == '}') {
                niveauAccolade--;
                if (niveauAccolade == 0) {
                    // Fin du bloc
                    listeContent[blocEnCours] = contenuBloc;
                    blocEnCours.clear(); // Réinitialiser le bloc en cours
                }
            }
        }

        // Recherche des clés spécifiques et extraction des valeurs
        if (ligne.find("\"mode\":") != std::string::npos) { // cherche la première occurrence de la sous-chaîne "mode" dans la chaîne ligne. npos : "pas de position valide" (not found).
            mode = std::stoi(ligne.substr(ligne.find(":") + 1));
        }
        if (ligne.find("\"tour\":") != std::string::npos) {
            tour = std::stoi(ligne.substr(ligne.find(":") + 1));
        }
        if (ligne.find("\"maxRetourArriere\":") != std::string::npos) {
            maxRetourArriere = std::stoi(ligne.substr(ligne.find(":") + 1));
        }
        if (ligne.find("\"modeIA\":") != std::string::npos) {
            modeIA = std::stoi(ligne.substr(ligne.find(":") + 1));
        }
    }
    this->tour = static_cast<unsigned int>(tour);
    this->modeIA = static_cast<int>(modeIA);
    this->mode = static_cast<int>(mode);
    this->maxRetourArriere = static_cast<int>(maxRetourArriere);
    std::cout<<"tour vaut !!!!" << tour;
    fichier.close(); // Ferme le fichier après lecture
    joueur1 = new JoueurHumain("undefined");
    joueur2 = new JoueurHumain("undefined2");
    Plateau plateau;
    if (mode == 1) {
        joueur2 = new JoueurHumain("undefined2");
    } else if (modeIA == 1) {
        joueur2 = new JoueurIA("IA");
    }else if (modeIA == 2){
        joueur2 = new JoueurIANiveau2("IA", &plateau.getPlateauMap(), &tour, poidsIA);
    }else if(modeIA == 3){
        joueur2 = new JoueurIANiveau3("IA", &plateau.getPlateauMap(), &tour, poidsIA, joueur1);
    }
    // Affiche les contenus des blocs détectés
    for (const auto& [bloc, contenu] : listeContent) {
        // Traitement spécifique pour les blocs "joueur1", "joueur2", "plateauMap"
        if (bloc == "joueur1" || bloc == "joueur2") {
            std::cout << "Traitement du bloc: " << bloc << std::endl;
            std::vector<Insecte*> deck;
            std::istringstream stream(contenu); // Utiliser un flux pour lire ligne par ligne , permet de simuler un getline
            std::string ligne;
            while (std::getline(stream, ligne)) {
                std::istringstream ligneStream(ligne); // même cas que au dessus, simuler un fichier pour parcourir la ligne caractère
                std::string element;
                int index = 1;
                std::vector<std::string> elementsEmpiles;
                while (std::getline(ligneStream, element, ';')) {
                    // Nettoyer les espaces autour de l'élément et supprimer les virgules
                    element = std::regex_replace(element, std::regex("^\\s+|\\s+$"), ""); // Enlever les espaces autour

                    // Nettoyer tout autre espace au milieu de la chaîne
                    element = std::regex_replace(element, std::regex("\\s+"), ""); // Supprimer les espaces internes supplémentaires
                    elementsEmpiles.push_back(element); // Empiler l'élément dans le vecteur
                    index++;
                }
                if (elementsEmpiles.size() >= 3){
                    if (joueurs.empty()){
                        joueurs[elementsEmpiles[3]] = joueur1;
                        joueur1->setName(elementsEmpiles[3]);
                        joueurs[elementsEmpiles[3]]->clearDeck();
                    } else if (joueurs.size() == 1 && joueurs.find(elementsEmpiles[3]) == joueurs.end()){
                        joueurs[elementsEmpiles[3]] = joueur2;
                        joueur2->setName(elementsEmpiles[3]);
                        joueurs[elementsEmpiles[3]]->clearDeck();
                    }
                    Hexagon hex(0,0);
                    Insecte *insecte = factory.createInsecte(elementsEmpiles[1] , hex ,joueurs[elementsEmpiles[3]]);
                    joueurs[elementsEmpiles[3]]->ajouterInsecte(insecte);
                }
            }
        }
        if (bloc == "insectesSurPlateau") {
            std::map<std::string , std::string> superpositions;
            std::cout << "Traitement du bloc: " << bloc << std::endl;

            // Flux pour parcourir le contenu du bloc
            std::istringstream stream(contenu);
            std::string ligne;

            // Parcourir chaque ligne du bloc
            while (std::getline(stream, ligne)) {
                std::vector<std::string> elementsEmpiles;
                std::istringstream ligneStream(ligne);
                std::string element;

                // Découper chaque ligne en utilisant ';' comme séparateur
                while (std::getline(ligneStream, element, ';')) {
                    // Nettoyer les espaces autour de chaque élément
                    element = std::regex_replace(element, std::regex("^\\s+|\\s+$"), "");
                    elementsEmpiles.push_back(element);
                }

                // Vérifier que les informations minimales sont présentes
                if (elementsEmpiles.size() >= 4) {
                    // Extraire les données nécessaires
                    std::string id = elementsEmpiles[0];
                    std::string type = elementsEmpiles[1];
                    std::string coords = elementsEmpiles[2];
                    std::string proprietaire = elementsEmpiles[3];
                    std::string dessus = elementsEmpiles[4];
                    std::string dessous = elementsEmpiles[5];

                    // Extraire les coordonnées Q et R à partir du format "[q,r]"
                    std::regex rgx("\\[(-?\\d+),(-?\\d+)\\]");
                    std::smatch matches;
                    int q = 0, r = 0;
                    if (std::regex_match(coords, matches, rgx)) {
                        q = std::stoi(matches[1].str());
                        r = std::stoi(matches[2].str());
                    } else {
                        std::cerr << "Erreur: Coordonnées mal formatées pour l'insecte " << id << std::endl;
                        continue;
                    }
                    if (joueurs.find(proprietaire) == joueurs.end()) {
                        if (joueurs.size() == 0) {
                            std::cout<<"Création du premier joueur 2 " << elementsEmpiles[3];
                            joueurs[proprietaire] = joueur1;
                            joueur1->setName(proprietaire);
                            joueurs[elementsEmpiles[3]]->clearDeck();
                        } else {
                            std::cout<<"Création du deuxieme joueur 2 " << elementsEmpiles[3];
                            joueurs[proprietaire] = joueur2;
                            joueur2->setName(proprietaire);
                            joueurs[elementsEmpiles[3]]->clearDeck();
                        }
                    }

                    // Créer l'insecte et l'ajouter au plateau
                    Insecte* insecte = factory.createInsecte(type, Hexagon(q, r), joueurs[proprietaire]);
                    mapID_Insecte[id] = insecte;
                    this->plateau.addInsectePlateau(insecte);
                    if (dessus != "null"){ //dessous est celui superposé
                        superpositions[id] = dessus;
                    } else {
                        pMap[Hexagon(q,r)] = insecte;
                    }
                } else {
                    std::cerr << "Erreur: Ligne mal formatée dans 'insectesSurPlateau': " << ligne << std::endl;
                }
            }
            for (const auto&[key, value] : superpositions){
                mapID_Insecte[key]->setDessus(mapID_Insecte[value]);
                mapID_Insecte[value]->setDessous(mapID_Insecte[key]);
            }
        }

    }
    return pMap;
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
    plateau.afficherPlateau(joueur1 , joueur2);
    bool hasWinner = false;
    hasWinner = detectWinner(joueur1, joueur2);
    while (!hasWinner) {
        Joueur* current = (tour % 2 == 0) ? joueur1 : joueur2;
        int playerTurn = (tour / 2) + 1; // Tour sp�cifique au joueur

        std::cout << "\nC'est au tour de : " << current->getName() << std::endl;
        if (current->hasQueen()) {
            int remainingTurnsToPlayQueen = 4 - playerTurn;
            std::cout << "Il vous reste " << remainingTurnsToPlayQueen
                      << " tours pour jouer votre reine." << std::endl;
        }

        int choice = 0;
        bool needPlayQueen = (playerTurn >= 4) && current->hasQueen();

        choice = current->getInputForAction();
        if (choice == 3){
            if (actionsDeque.size() >= 2) {
                    undoLastTwoActions();
                    break;
            } else {
                std::cout << "Aucune action � annuler, essayez une autre option.\n";
                choice = current->getInputForAction();
                continue;
            }
        } else {
            if (needPlayQueen) {
                std::cout << "Vous devez obligatoirement poser votre Reine !\n";
                choice = 2; // Forcer le choix de poser la reine
            } else {
                if (!plateau.playerCanMoveInsecte(current) && current->getDeckSize() > 0) {
                    std::cout << "Aucun mouvement possible, vous devez placer un pion.\n";
                    choice = 2;
                }
            }
        }
        if (current->getDeckSize() == 0 && choice != 3) choice = 1;
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
        saveGame();
    }
}

void GameMaster::placerPion(Joueur* current, bool needPlayQueen) {
    Insecte* insecteAPlacer = nullptr;
    int index = 0;
    // S�lection du pion � placer
    while (insecteAPlacer == nullptr){
        if (current->getDeckSize()==0) break;
        if (!needPlayQueen) {
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
            if (placementsPossibles.empty() && plateau.playerCanMoveInsecte(current)){
                placementsPossibles = insecteAPlacer->getCoords().getVoisins();
            }
            plateau.afficherPlateauAvecPossibilites(placementsPossibles, joueur1, joueur2, current);
            plateau.afficherPossibilitesPlacements(insecteAPlacer, placementsPossibles);;
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
    action->executerAction(plateau);
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
    jsonData << "  \"maxRetourArriere\": " << maxRetourArriere << ",\n";
    jsonData << "  \"modeIA\": " << modeIA << "\n";

    jsonData << "}";
    jsonData << "  \"actions\"{\n " ;
    for (auto it = actionsDeque.begin() ; it != actionsDeque.end() ; it++){
        jsonData << (*it)->toJson() << "\n";
    }
    jsonData << "}" ;
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








