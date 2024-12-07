#pragma ONCE
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

// ActionManager gère les actions possibles
class ActionManager {
private:
    std::unordered_map<std::string, std::function<std::unique_ptr<Action>(Insecte*, Hexagon, Joueur*)>> actionFactories;

public:
    void ajouterExtensionAction(const std::string& nomAction, std::function<std::unique_ptr<Action>(Insecte*, Hexagon, Joueur*)> creator) {
        actionFactories[nomAction] = creator;
    }

    std::unique_ptr<Action> createAction(const std::string& nomAction, Insecte* insecte, Hexagon position, Joueur* owner) {
        auto it = actionFactories.find(nomAction);
        if (it != actionFactories.end()) {
            return it->second(insecte, position, owner);
        }
        return nullptr;  // Action inconnue
    }
};

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
        : insecte(insecte), position(position), joueur(joueur), insectBelow(nullptr) {}

    void executerAction(Plateau& plateau) override ;
    void undo(Plateau& plateau) override;

    std::string toJson() const override ;
};

class DeplacementAction : public Action {
private:
    Insecte* insecte;
    Hexagon oldPosition;
    Hexagon newPosition;

public:
    DeplacementAction(Insecte* insecte, const Hexagon& oldPos, const Hexagon& newPos)
        : insecte(insecte), oldPosition(oldPos), newPosition(newPos) {}

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

#endif // ACTION_H
