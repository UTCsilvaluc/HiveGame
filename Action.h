#ifndef ACTION_H
#define ACTION_H
#include "Insecte.h"
#include "Plateau.h"
#include "Joueur.h"
#include <functional>
#include <unordered_map>
#include <memory>
#include <iostream>

// Déclaration anticipée de la classe Action
class Action;
class Plateau;

// Classe de base pour les actions
class Action {
public:
    virtual ~Action() = default;

    // Méthodes virtuelles pures
    virtual void executerAction(Plateau& plateau) = 0;
    virtual void undo(Plateau& plateau) = 0;
    virtual std::string toJson() const = 0;
};

// Actions spécifiques héritées de Action
class PlacementAction : public Action {
private:
    Insecte* insecte;
    Hexagon position;
    Joueur* joueur;
    Insecte* insectBelow;

public:
    PlacementAction(Insecte* insecte, const Hexagon& position, Joueur* joueur)
        : insecte(insecte), position(position), joueur(joueur) {}

    void executerAction(Plateau& plateau) override ;
    void undo(Plateau& plateau) override;

    std::string toJson() const override ;
};

class DeplacementAction : public Action {
private:
    Insecte* insecte;
    Hexagon oldPosition;
    Hexagon newPosition;
    Insecte* insectBelow;

public:
    DeplacementAction(Insecte* insecte, const Hexagon& oldPos, const Hexagon& newPos)
        : insecte(insecte), oldPosition(oldPos), newPosition(newPos), insectBelow(nullptr) {}

    void executerAction(Plateau& plateau) override;

    void undo(Plateau& plateau) override;

    std::string toJson() const override;
};

class MangerPionAction : public Action {
private:
    Insecte* insecte;           // L'insecte qui effectue l'action
    Hexagon oldPosition;        // La position initiale de l'insecte
    Hexagon newPosition;        // La nouvelle position de l'insecte
    Insecte* insectToRemove;    // L'insecte qui sera mangé et supprimé

public:
    MangerPionAction(Insecte* insecte, const Hexagon& oldPos, const Hexagon& newPos)
        : insecte(insecte), oldPosition(oldPos), newPosition(newPos), insectToRemove(nullptr) {}

    void executerAction(Plateau& plateau) override;

    void undo(Plateau& plateau) override ;

    std::string toJson() const override ;
};

class ActionFactory {
public:
    // Méthode statique pour créer une action en fonction du nom
    static std::unique_ptr<Action> createAction(const std::string& actionType,
                                                Insecte* insecte,
                                                const Hexagon& position,
                                                Joueur* joueur,
                                                const Hexagon& oldPos = Hexagon(),
                                                const Hexagon& newPos = Hexagon()) {
        if (actionType == "PlacementAction") {
            return std::make_unique<PlacementAction>(insecte, position, joueur);
        } else if (actionType == "DeplacementAction") {
            return std::make_unique<DeplacementAction>(insecte, oldPos, newPos);
        } else if (actionType == "MangerPionAction") {
            return std::make_unique<MangerPionAction>(insecte, oldPos, newPos);
        } else {
            throw std::invalid_argument("Type d'action inconnu");
        }
    }
};

#endif // ACTION_H
