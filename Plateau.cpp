#include "Plateau.h"
#include <iomanip>
void Plateau::ajouterInsecte(Insecte* insecte, Hexagon position) {
    if (plateauMap.count(position)) {
        Insecte* insecteEnDessous = plateauMap[position];
        superposerInsecte(insecteEnDessous, insecte);
        std::cout << "Pion place avec succes en " << position << "." << std::endl;
    } else {
        plateauMap[position] = insecte;
        insecte->setCoords(position);
        mettreAJourLimites();
        std::cout << "Pion place avec succes en " << position << "." << std::endl;
    }
    addInsectePlateau(insecte);
}

void Plateau::deplacerInsecte(Insecte* insecte, const Hexagon& nouvellePosition) {
    bool anciennePosHasDessous = false;
    Hexagon ancienneCoords = insecte->getCoords();
    // Vérifier si un insecte existe déjà à la nouvelle position
    if (plateauMap.count(nouvellePosition)) {
        if (insecte->getDessous() != nullptr){
            anciennePosHasDessous = true;
            Insecte* dessous = insecte->getDessous();
            plateauMap[dessous->getCoords()] = dessous;
            dessous->setDessus(nullptr);
            insecte->setDessous(nullptr);
            //insecte->setCoords(nouvellePosition);
            //plateauMap[nouvellePosition] = insecte;
        }
        if (anciennePosHasDessous == false){
            plateauMap.erase(ancienneCoords);
        }
        Insecte* insecteExistant = plateauMap[nouvellePosition];
        superposerInsecte(insecteExistant, insecte);
    } else {
        if (insecte->getDessous() != nullptr){
            Insecte* dessous = insecte->getDessous();
            plateauMap[dessous->getCoords()] = dessous;
            dessous->setDessus(nullptr);
            insecte->setDessous(nullptr);
            insecte->setCoords(nouvellePosition);
            plateauMap[nouvellePosition] = insecte;
        } else {
            plateauMap.erase(insecte->getCoords());
            insecte->setCoords(nouvellePosition);
            plateauMap[nouvellePosition] = insecte;
        }
    }
    mettreAJourLimites(); // Mettre à jour les limites du plateau
}
void Plateau::superposerInsecte(Insecte* insecteExistant, Insecte* newInsecte) {
    newInsecte->setDessous(insecteExistant);
    insecteExistant->setDessus(newInsecte);
    plateauMap[insecteExistant->getCoords()] = newInsecte;
    newInsecte->setCoords(insecteExistant->getCoords());
    mettreAJourLimites();
}

void Plateau::afficherPlateau(Joueur *p1, Joueur *p2) const {
    for (int r = minR; r <= maxR; ++r) {
        // D�calage pour les lignes impaires pour cr�er l'effet hexagonal
        if (r % 2 != 0) {
            std::cout << "   "; // D�calage pour simuler la forme hexagonale
        }

        std::string couleurInsecte = RESET;
        std::string couleurCoord = RESET;
        for (int q = minQ; q <= maxQ; ++q) {
            Hexagon h(q, r);
            couleurInsecte = RESET;
            couleurCoord = RESET;

            // Affiche l'insecte si pr�sent, sinon un point pour une case vide
            if (plateauMap.count(h)) {
                // R�cup�ration de l'insecte sur la case
                Insecte *insecte = plateauMap.at(h);
                std::string nomInsecte = insecte->getNom();

                // Changer la couleur en fonction du propri�taire de l'insecte
                if (insecte->getOwner() == p1) {
                    couleurInsecte = RED;
                    couleurCoord = RED;
                } else if (insecte->getOwner() == p2) {
                    couleurInsecte = BLUE;
                    couleurCoord = BLUE;
                }

                // Si c'est un l'affichage des placements/deplacements possibles
                if (dynamic_cast<InsecteFictif*>(insecte))
                    couleurCoord = GREEN;

                // Prendre seulement la premi�re lettre pour �viter un affichage trop large
                std::cout << couleurInsecte << nomInsecte[0] << couleurCoord << "[" << h.getQ() << "," << h.getR() << "] ";
            } else {
                // Affichage d'un point pour une case vide
                std::cout << ".      ";
            }
        }

        // Reset de la couleur et retour � la ligne apr�s chaque rang�e
        std::cout << RESET << std::endl << std::endl;
    }
}

std::vector<Hexagon> Plateau::getVoisinsInsectePlateau(Insecte *insecte) {
    std::vector<Hexagon> voisins;
    int q = insecte->getCoords().getQ();
    int r = insecte->getCoords().getR();
    if (r % 2 == 0) {  // Colonne paire
        voisins = {
                Hexagon(q - 1, r - 1),
                Hexagon(q, r - 1),
                Hexagon(q + 1, r),
                Hexagon(q, r + 1),
                Hexagon(q - 1, r + 1),
                Hexagon(q - 1, r)
        };
    } else {  // Colonne impaire
        voisins = {
                Hexagon(q, r - 1),
                Hexagon(q + 1, r - 1),
                Hexagon(q+1, r),
                Hexagon(q+1, r+1),
                Hexagon(q, r+1),
                Hexagon(q-1, r)
        };
    }
    return voisins;
}

Insecte* Plateau::getReineAbeille(Joueur* joueur) const { // ou utiliser insectesSurPlateau
    for (const auto& pair : plateauMap) {
        const auto& value = pair.second;
        if (value->getNom() == "Reine" && value->getOwner() == joueur) {
            return value;
        }
    }
    return nullptr;
}
bool Plateau::estEntouree(Insecte *insecte) {
    std::vector<Hexagon> voisins = getVoisinsInsectePlateau(insecte);
    std::cout << "Voisins de l'insecte (" << insecte->getCoords().getQ() << ", " << insecte->getCoords().getR() << ") :\n";

    for (const Hexagon& voisin : voisins) {
        auto it = plateauMap.find(voisin);

        if (it == plateauMap.end()) {
            std::cout << "Case vide (hors du plateau ou non initialis�e).\n";
            return false;
        }

        if (it->second == nullptr) {
            std::cout << "Case pr�sente mais aucun insecte.\n";
            return false;
        }

        // La case est occup�e par un insecte
        std::cout << "Case occup�e par : " << it->second->getNom() << "\n";
    }


    return true; // Toutes les cases voisines sont occup�es
}


bool Plateau::plateauEstVide(){
    return (plateauMap.size() == 0);
}

bool Plateau::playerCanMoveInsecte(Joueur* joueur) {
    // Parcours de tous les insectes dans le plateau
    for (const auto& pair : plateauMap) {
        const auto& insecte = pair.second;

        // Vérifie si l'insecte appartient au joueur
        if (insecte->getOwner() == joueur) {
            // Affichage des informations sur l'insecte
            std::cout << "Insecte: " << insecte->getNom() << " à la position ("
                      << insecte->getCoords().getQ() << ", "
                      << insecte->getCoords().getR() << ")\n";

            // Vérification des déplacements possibles de cet insecte
            std::vector<Hexagon> deplacementsPossibles = insecte->deplacementsPossibles(plateauMap);

            // Affichage des déplacements possibles
            if (!deplacementsPossibles.empty()) {
                std::cout << "Déplacements possibles pour " << insecte->getNom() << " : ";
                for (const Hexagon& hex : deplacementsPossibles) {
                    std::cout << "(" << hex.getQ() << ", " << hex.getR() << ") ";
                }
                std::cout << std::endl;
                return true;  // Si au moins un déplacement est possible, retourner true
            } else {
                std::cout << "Aucun déplacement possible pour " << insecte->getNom() << std::endl;
            }
        }
    }

    // Si aucun insecte du joueur n'a de déplacements possibles
    std::cout << "Aucun insecte du joueur n'a de déplacements possibles." << std::endl;
    return false;
}



Insecte* Plateau::getSeulInsecteSurPlateau() const {
    if (plateauMap.size() == 1) {
        return plateauMap.begin()->second; // Retourne le premier (et seul) insecte
    }
    return nullptr; // Retourne nullptr si aucun ou plusieurs insectes
}
void Plateau::supprimerInsecte(const Hexagon& position) {
    plateauMap.erase(position);
}
const std::map<Hexagon, Insecte*>& Plateau::getPlateauMap() const {
    return plateauMap;
}
void Plateau::supprimerInsectePlateauCoords(const Hexagon& position) {
    auto it = plateauMap.find(position);
    if (it == plateauMap.end()) {
        std::cerr << "Erreur : Aucun insecte trouv� � la position (" << position.getQ()
                  << ", " << position.getR() << ")." << std::endl;
        return;
    }
    Insecte* insecteASupprimer = it->second;
    plateauMap.erase(it); // Supprimer l'insecte de la map

    // Retirer l'insecte de la liste des insectes sur le plateau
    mettreAJourLimites();
}
std::string escapeJson(const std::string& input) {
    std::ostringstream escaped;
    for (char c : input) {
        switch (c) {
            case '\"': escaped << "\\\""; break; // Échapper les guillemets
            case '\\': escaped << "\\\\"; break; // Échapper les barres obliques inverses
            case '\b': escaped << "\\b"; break;  // Échapper backspace
            case '\f': escaped << "\\f"; break;  // Échapper form feed
            case '\n': escaped << "\\n"; break;  // Échapper nouvelle ligne
            case '\r': escaped << "\\r"; break;  // Échapper retour chariot
            case '\t': escaped << "\\t"; break;  // Échapper tabulation
            default:
                if (c < 0x20 || c > 0x7E) { // Échapper les caractères non imprimables
                    escaped << "\\u"
                            << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    escaped << c; // Ajouter tel quel
                }
        }
    }
    return escaped.str();
}

std::string Plateau::toJson() const {
    std::stringstream jsonData;

    jsonData << "{\n";
    jsonData << "  \"insectesSurPlateau\": {\n";
    for (auto it = insectesSurPlateau.begin() ; it != insectesSurPlateau.end() ; it++){
        jsonData << (*it)->toJson() << "\n";
    }
    jsonData << "\n  },\n";

    // nombreRetoursArriere
    jsonData << "  \"nombreRetoursArriere\": " << nombreRetoursArriere << ",\n";

    // minR, maxR, minQ, maxQ
    jsonData << "  \"minR\": " << minR << ",\n";
    jsonData << "  \"maxR\": " << maxR << ",\n";
    jsonData << "  \"minQ\": " << minQ << ",\n";
    jsonData << "  \"maxQ\": " << maxQ << "\n";

    jsonData << "}";
    return jsonData.str();
}

void Plateau::afficherPossibilitesDeplacements(const Insecte* insecte, const std::vector<Hexagon> deplacementsPossibles) const {
    std::cout << "Déplacements possibles pour " << insecte->getNom() << " : \n";
    for (size_t i = 0; i < deplacementsPossibles.size(); ++i) {
        std::cout << i + 1 << ". (" << deplacementsPossibles[i].getQ() << ", " << deplacementsPossibles[i].getR() << ")\n";
    }
    std::cout << "-1. Annuler le déplacement\n";

}
void Plateau::afficherPossibilitesPlacements(const Insecte* insecte, const std::vector<Hexagon> placementsPossibles) const{
    std::cout << "Placements possibles pour " << insecte->getNom() << " : \n";
    for (size_t i = 0; i < placementsPossibles.size(); ++i) {
        std::cout << i + 1 << ". (" << placementsPossibles[i].getQ() << ", " << placementsPossibles[i].getR() << ")\n";
    }
    std::cout << "-1. Annuler le placement\n";
}

std::vector<Hexagon> Plateau::getPlacementsPossibles(Insecte* insecte) {
    std::vector<Hexagon> placements;
    if (plateauMap.size() == 1) {
        Insecte* seulInsecte = getSeulInsecteSurPlateau();
        if (seulInsecte == nullptr) {
            std::cerr << "Erreur : Aucun insecte trouve sur le plateau." << std::endl;
            return {};  // Retourne un vecteur vide en cas d'erreur.
        }
        placements = getVoisins(seulInsecte->getCoords());
    }else {
        placements = insecte->placementsPossiblesDeBase(plateauMap);
    }
    return placements;
}

void Plateau::afficherPlateauAvecPossibilites(const std::vector<Hexagon>& emplacementsPossibles, Joueur* j1, Joueur* j2, Joueur* current) {
    // Cr�er une copie temporaire du plateau pour l'affichage
    std::map<Hexagon, Insecte*> plateauTemp = plateauMap;
    // Ajouter des insectes fictifs aux emplacements possibles dans le plateau temporaire
    for (const Hexagon& hex : emplacementsPossibles) {
        if (plateauTemp.find(hex) == plateauTemp.end()) {  // Si la position est vide
            plateauTemp[hex] = new InsecteFictif(hex, current);  // Ajouter un insecte fictif
        }
    }
    // Afficher le plateau avec les insectes fictifs en utilisant afficherPlateau
    std::swap(plateauMap, plateauTemp);  // �change temporairement plateauMap et plateauTemp
    mettreAJourLimites();
    afficherPlateau(j1, j2);
    std::swap(plateauMap, plateauTemp);  // Restaure plateauMap
    // Nettoyer les insectes fictifs ajout�s dans le plateau temporaire
    for (const Hexagon& hex : emplacementsPossibles) {
        if (plateauTemp[hex]->getNom() == "?") {  // V�rifie si c'est un insecte fictif
            delete plateauTemp[hex];
            plateauTemp.erase(hex);
        }
    }
}
