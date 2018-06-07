#ifndef GRILLE_H
#define GRILLE_H

#include "cell.h"
#include "etat.h"

class Grille
{
    Cell** valeurs;
    unsigned int taille;
public:
    Grille(unsigned int t): taille(t) {
        valeurs = new Cell*[taille];

        for (unsigned int i = 0; i < taille; i++) {
            valeurs[i] = new Cell[taille];

            for (unsigned int j = 0; j < taille; j++) {
                Cell* newCell = new Cell(etatsPossibles[0]);
                grilles[i][j][k] = *newCell;
            }
        }
    }

    // Set cell here
    void setCell(unsigned i, unsigned int j, Cell& c);
    const unsigned int getTaille() const { return taille; }
};

#endif // GRILLE_H