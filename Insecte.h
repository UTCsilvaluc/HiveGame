#ifndef INSECTE_H
#define INSECTE_H

#include "Hexagon.h"
#include "Action.h"
#include "Plateau.h"
#include <string>
#include <iostream>
#include <functional>
#include <map>
#include <sstream>  // Pour std::stringstream
#include <memory>
#include <set>
#include <algorithm>  // For std::find

class Joueur; // Déclaration anticipée de Joueur pour éviter l'inclusion circulaire
class Action;
class Insecte
{
private:
    Hexagon coords;
    Insecte *dessus = nullptr;
    Insecte *dessous = nullptr;
    Joueur *owner;
    std::string nom;
    std::function<void()> callback; // Fonction callback pour des comportements personnalisés

public:
    Insecte& operator =(const Insecte &i){
        if(this != &i){
            coords = i.getCoords();
            dessus = i.dessus;
            dessous = i.dessous;
            owner = i.owner;
            nom = i.getNom();
        }
        return *this;
    }

    Insecte(std::string nom, Hexagon coords, Joueur *owner) : coords(coords), nom(nom), owner(owner), callback(nullptr) {}

    Hexagon getCoords() const { return coords; }
    std::string getNom() const { return nom; }
    const Joueur* getOwner() const { return owner; }
    virtual bool isQueen() const {return false;} // Par défaut, un insecte n'est pas une Reine
    std::vector<Hexagon> getVoisinsEnnemis(const std::vector<Hexagon>& voisins, const std::map<Hexagon, Insecte*>& plateau) const;
    void setDessus(Insecte *insecte){
        dessus = insecte;
    }
    void setDessous(Insecte *insecte){
        dessous = insecte;
    }
    void setCoords(Hexagon newCoords){
        coords = newCoords;
    }
    Insecte* getDessus(){
        return dessus;
    }
    Insecte* getDessous(){
        return dessous;
    }
    std::string toJson() const;

    virtual std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p) = 0;

    std::string getFirstCarac() {
        return std::string(1, this->nom[0]);
    }

    std::vector<Hexagon> placementsPossiblesDeBase(const std::map<Hexagon, Insecte*>& plateau) const;

    std::vector<Hexagon> getPlacementsPossibles(const std::map<Hexagon, Insecte*>& plateau) const;

    void setCallback(std::function<void()> cb) { callback = cb; }
    void executeCallback() { if (callback) callback(); }

    // Méthodes pour exécuter les actions par défaut
    virtual Action* actionPlacer(Hexagon targetCoord);

    virtual Action* actionDeplacer(Hexagon targetCoord);
};

// Adaptation des constructeurs pour chaque classe d'insecte dérivée
class ReineAbeille : public Insecte {
public:
    ReineAbeille(Hexagon coords, Joueur *owner) : Insecte("Reine", coords, owner) {}
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p) override;
    bool estEntouree(const std::map<Hexagon, Insecte*>& p) const;
    bool isQueen() const override {return true;} // ReineAbeille est une Reine
};

class Fourmi : public Insecte {
public:
    Fourmi(Hexagon coords, Joueur *owner) : Insecte("Fourmi", coords, owner) {}
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p);
};

class Sauterelle : public Insecte {
public:
    Sauterelle(Hexagon coords, Joueur *owner) : Insecte("Sauterelle", coords, owner) {}
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p);
};

class Coccinelle : public Insecte {
public:
    Coccinelle(Hexagon coords, Joueur *owner) : Insecte("Coccinelle", coords, owner) {}
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p);
};

class Scarabee : public Insecte {
public:
    Scarabee(Hexagon coords, Joueur *owner) : Insecte("Scarabee", coords, owner) {}
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p);
};

class Araignee : public Insecte {
public:
    Araignee(Hexagon coords, Joueur *owner) : Insecte("Araignée", coords, owner) {}
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p);
};

class Moustique : public Insecte {
public:
    Moustique(Hexagon coords, Joueur *owner) : Insecte("Moustique", coords, owner) {}
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p);
};
class Termite : public Insecte {
public:
    Termite(Hexagon coords, Joueur* owner) : Insecte("Termite", coords, owner) {}

    Action* actionDeplacer(Hexagon targetCoord) override;

    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p) override;
};

class InsecteFictif : public Insecte {
public:
    InsecteFictif(Hexagon coords, Joueur* player) : Insecte("X", coords, player) {}
    // Implémentation correcte de la fonction pure virtuelle
    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p) override {
        return {}; // Pas de déplacement pour cet insecte fictif
    }
};

class Papillon : public Insecte {
public:
    Papillon(Hexagon coords, Joueur* owner) : Insecte("Papillon", coords, owner) {}


    std::vector<Hexagon> deplacementsPossibles(std::map<Hexagon, Insecte*> p) override {
        return {};
    }
};

// Interface Factory
class InsecteFactory {
public:
    virtual Insecte* createInsecte(const std::string& type, Hexagon coords, Joueur* owner) = 0;
};

class InsecteFactoryImpl {
private:
    struct InsecteConfig {
        std::function<Insecte*(Hexagon, Joueur*)> creator;  // Fonction de création
        int occurrences;                                    // Nombre d'occurrences par défaut
    };

    std::map<std::string, InsecteConfig> insecteUtilisee;   // Insectes utilisés et leurs occurrences
    std::map<std::string, InsecteConfig> insecteCreators;  // Tous les insectes disponibles
    std::set<std::string> insecteExtensionsActives;        // Extensions activées
    unsigned int nbMaxInsecte;
public:
    InsecteFactoryImpl(unsigned int maxInsecte = 13) : nbMaxInsecte(maxInsecte) {
        // Insectes de base
        ajouterInsecteBase("Reine", [](Hexagon coords, Joueur* owner) { return new ReineAbeille(coords, owner); }, 1);
        ajouterInsecteBase("Fourmi", [](Hexagon coords, Joueur* owner) { return new Fourmi(coords, owner); }, 3);
        ajouterInsecteBase("Sauterelle", [](Hexagon coords, Joueur* owner) { return new Sauterelle(coords, owner); }, 3);
        ajouterInsecteBase("Coccinelle", [](Hexagon coords, Joueur* owner) { return new Coccinelle(coords, owner); }, 2);
        ajouterInsecteBase("Scarabee", [](Hexagon coords, Joueur* owner) { return new Scarabee(coords, owner); }, 2);
        ajouterInsecteBase("Araignée", [](Hexagon coords, Joueur* owner) { return new Araignee(coords, owner); }, 2);
        ajouterInsecteBase("Moustique", [](Hexagon coords, Joueur* owner) { return new Moustique(coords, owner); }, 1);

        // Ajouter des extensions
        ajouterExtensionInsecte("Papillon", [](Hexagon coords, Joueur* owner) { return new Papillon(coords, owner); }, 1);
        ajouterExtensionInsecte("Termite", [](Hexagon coords, Joueur* owner) { return new Termite(coords, owner); }, 1);

        // Initialisation des insectes joués
        initialiserInsectesDeBase();
    }
    void initialiserInsectesDeBase() {
        insecteUtilisee = insecteCreators;  // Copie les insectes de base comme jouables
    }

    // Ajouter un insecte de base
    void ajouterInsecteBase(const std::string& nom, std::function<Insecte*(Hexagon, Joueur*)> creator, int occurrences) {
        insecteCreators[nom] = {creator, occurrences};
    }

    Insecte* createInsecte(const std::string& type, Hexagon coords, Joueur* owner) {
        if (insecteCreators.find(type) != insecteCreators.end()) {
            // Accéder à la fonction créatrice
            return insecteCreators[type].creator(coords, owner);
        }
        return nullptr;
    }
    // Liste des extensions disponibles (non activées)
    std::vector<std::string> getInsecteExtensionsDisponibles() const {
        std::vector<std::string> extensionsDisponibles;
        for (const auto& [nom, _] : insecteCreators) {
            if (insecteExtensionsActives.find(nom) == insecteExtensionsActives.end()) {
                extensionsDisponibles.push_back(nom);
            }
        }
        return extensionsDisponibles;
    }


    // Création d'un insecte pour une extension activée
    std::vector<Insecte*> createInsecteExtension(const std::string& nomExtension, Hexagon coords, Joueur* owner, int count) {
        std::vector<Insecte*> insectes;
        if (insecteExtensionsActives.find(nomExtension) != insecteExtensionsActives.end()) {
            auto creator = insecteCreators.find(nomExtension);
            if (creator != insecteCreators.end()) {
                for (int i = 0; i < count; ++i) {
                    insectes.push_back(creator->second.creator(coords, owner));
                }
            }
        }
        return insectes;
    }

    // Ajouter une nouvelle extension à la liste des extensions disponibles
    void ajouterExtensionInsecte(const std::string& nom, std::function<Insecte*(Hexagon, Joueur*)> creator, int occurrences) {
        if (insecteCreators.find(nom) == insecteCreators.end()) {
            insecteCreators[nom] = {creator, occurrences};
        }
    }

    // Activer une extension d'insecte (permet d'utiliser cette extension dans le jeu)
    void activerExtension(const std::string& nomExtension) {
        if (insecteCreators.find(nomExtension) != insecteCreators.end()) {
            insecteExtensionsActives.insert(nomExtension);
            insecteUtilisee[nomExtension] = insecteCreators[nomExtension];
        }
    }

    // Désactiver une extension d'insecte (ne permet plus de l'utiliser)
    void desactiverExtension(const std::string& nomExtension) {
        insecteExtensionsActives.erase(nomExtension);
        insecteUtilisee.erase(nomExtension);
    }
    void ajouterOccurrence(const std::string& nomInsecte) {
        if (insecteUtilisee.find(nomInsecte) != insecteUtilisee.end()) {
            ++insecteUtilisee[nomInsecte].occurrences;
        }
    }
    void retirerOccurrence(const std::string& nomInsecte) {
        if (insecteUtilisee.find(nomInsecte) != insecteUtilisee.end() && nomInsecte != "Reine") {
            if (--insecteUtilisee[nomInsecte].occurrences <= 0) {
                insecteUtilisee.erase(nomInsecte);  // Supprimer si occurrences à zéro
            }
        }
    }
    std::vector<Insecte*> genererDeck(Hexagon coords, Joueur* owner) {
        std::vector<Insecte*> deck;
        for (const auto& [nom, config] : insecteUtilisee) {
            for (int i = 0; i < config.occurrences; ++i) {
                deck.push_back(config.creator(coords, owner));
            }
        }
        return deck;
    }
    void afficherInsectes() const {
        for (const auto& [nom, config] : insecteUtilisee) {
            std::cout << "- " << nom << " (Occurrences : " << config.occurrences << ")\n";
        }
    }

    // Liste des extensions activées
    std::vector<std::string> getInsecteExtensionsActives() {
        return std::vector<std::string>(insecteExtensionsActives.begin(), insecteExtensionsActives.end());
    }

    // Liste des extensions disponibles (non activées)
    bool estExtensionActive(const std::string& nomExtension) {
        return insecteExtensionsActives.find(nomExtension) != insecteExtensionsActives.end();
    }
};

// Fonctions supplémentaires
std::vector<Hexagon> deplacementsPossiblesReineAbeille(Hexagon coords, std::map<Hexagon, Insecte*> p);
void deplacementsPossiblesFourmi(Hexagon coords, std::map<Hexagon, Insecte*> p, std::vector<Hexagon>& cheminInsecte, std::set<Hexagon>& deplacements);
void deplacementsPossiblesSauterelle(Hexagon coords, std::map<Hexagon, Insecte*> p);
void deplacementsPossiblesCoccinelle(Hexagon coords, std::map<Hexagon, Insecte*> p, int i, std::vector<Hexagon> &cheminFinal, std::set<Hexagon> &visited);
std::vector<Hexagon> deplacementsPossiblesScarabee(Hexagon coords, std::map<Hexagon, Insecte*> p);
void deplacementsPossiblesAraignee(Hexagon coords, std::map<Hexagon, Insecte*> p, int i, std::vector<Hexagon>& cheminInsecte, std::set<Hexagon>& deplacementsFinaux);
std::vector<Hexagon> deplacementsPossiblesMoustique(Hexagon coords, std::map<Hexagon, Insecte*> p);
std::vector<Hexagon> getVoisins(const Hexagon& coords);
std::vector<Hexagon> casesAdjacentesVides(Hexagon coords, const std::map<Hexagon, Insecte*>& p);
std::vector<Hexagon> casesAdjacentesOccupees(Hexagon coords, const std::map<Hexagon, Insecte*>& p);
void getLongueurChaine(Hexagon coords, std::map<Hexagon, Insecte*> p, std::set<Hexagon> &chemin);
bool getChaineBrisee(Hexagon coords, std::map<Hexagon, Insecte*> p, std::set<Hexagon> &chemin);
bool getGlissementPossible(const Insecte *i, const std::map<Hexagon, Insecte*> p, const Hexagon destination);
Insecte* trouverReine(Joueur* joueur, const std::map<Hexagon, Insecte*>& plateau);
Hexagon getInsectPosition(const std::map<Hexagon, Insecte*>& plateau, Insecte* insecte);



#endif // INSECTE_H
