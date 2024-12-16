#include "Action.h"
// Ajouter des actions spécifiques au gestionnaire
void PlacementAction::executerAction(Plateau& plateau) {
    plateau.ajouterInsecte(insecte, position);
    if (plateau.getInsecteAt(position)) {
        insectBelow = plateau.getInsecteAt(position);
    }
}

void PlacementAction::undo(Plateau& plateau) {
    if (!insecte) {
        std::cerr << "Erreur : Aucun insecte à annuler pour le placement !" << std::endl;
        return;
    }

    plateau.supprimerInsectePlateauCoords(position);
    plateau.removeInsectePlateau(insecte);
    if (joueur) {
        joueur->ajouterInsecte(insecte);
    }

    if (insectBelow) {
        insectBelow->setDessus(nullptr);
    }
}
std::string PlacementAction::toJson() const {
    std::stringstream jsonData;
    jsonData << "\"PlacementAction\";";
    jsonData << position.toJson() << ";";
    jsonData << (joueur ? joueur->getName() : "null") << ";";
    jsonData << "("<< (insecte ? insecte->toJson() : "null")<< ");";
    jsonData << "("<< (insectBelow ? insectBelow->toJson() : "null")<< ")";
    return jsonData.str();
}
std::string DeplacementAction::toJson() const {
    std::stringstream jsonData;
    jsonData << "\"DeplacementAction\";";
    jsonData << "("<< (insecte ? insecte->toJson() : "null") << ");";
    jsonData << "(" << (insectBelow ? insectBelow->toJson() : "null")<< ");";
    jsonData << oldPosition.toJson() << ";";
    jsonData << newPosition.toJson();
    return jsonData.str();
}

std::string MangerPionAction::toJson() const {
    std::stringstream jsonData;
    jsonData << "\"MangerPionAction\";";
    jsonData << "(" << (insecte ? insecte->toJson() : "null") << ");";
    jsonData << oldPosition.toJson() << ";";
    jsonData << newPosition.toJson() << ";";
    jsonData << "(" << (insectToRemove ? insectToRemove->toJson() : "null") << ")";
    return jsonData.str();
}

void DeplacementAction::executerAction(Plateau& plateau)  {
    if (plateau.getInsecteAt(newPosition) != nullptr){
        insectBelow = plateau.getInsecteAt(newPosition);
    }
    plateau.deplacerInsecte(insecte, newPosition);
}

void DeplacementAction::undo(Plateau& plateau)  {
    if (!insecte) {
        std::cerr << "Erreur : Aucun insecte à annuler pour le déplacement !" << std::endl;
        return;
    }
    plateau.deplacerInsecte(insecte, oldPosition);
}

void MangerPionAction::executerAction(Plateau& plateau) {
    insectToRemove = plateau.getInsecteAt(newPosition);
    if (insectToRemove) {
        plateau.supprimerInsectePlateauCoords(newPosition);
    }
    plateau.deplacerInsecte(insecte, newPosition);
}

void MangerPionAction::undo(Plateau& plateau) {
    // Révertir le déplacement de l'insecte à sa position initiale
    plateau.deplacerInsecte(insecte, oldPosition);
    // Réajouter l'insecte mangé à sa position précédente
    if (insectToRemove) {
        plateau.ajouterInsecte(insectToRemove, newPosition);
    }
}
