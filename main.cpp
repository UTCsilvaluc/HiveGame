#include "GameMaster.h"
int main() {
    Plateau plateau;
    /*
    plateau.ajouterInsecte(new ReineAbeille(Hexagon(0, 0)));
    plateau.ajouterInsecte(new Fourmi(Hexagon(1, 0)));
    plateau.ajouterInsecte(new Sauterelle(Hexagon(-1, 1)));
    plateau.ajouterInsecte(new Fourmi(Hexagon(0, 1)));
    plateau.ajouterInsecte(new Coccinelle(Hexagon(1, 1)));
    plateau.afficherPlateau();*/

    GameMaster master(plateau);
    master.startGameForIA();
    // Test

    return 0;
}
