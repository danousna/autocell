#ifndef GRILLE_H
#define GRILLE_H

#include "cell.h"

class Grille
{
    Cell** valeurs;
    unsigned int taille;
	unsigned int dimension;
public:
	Grille(){};

    Grille(unsigned int t, unsigned int d): taille(t),dimension(d) {
        valeurs = new Cell*[taille];

        if (d == 1) {
            valeurs[0] = new Cell[taille];

            for (unsigned int i = 0; i < taille; i++) {
                Cell* newCell = new Cell();
                valeurs[0][i] = *newCell;
            }

            for (unsigned int i = 1; i < taille; i++) {
                valeurs[i] = nullptr;
            }
        } else if (d == 2) {
            for (unsigned int i = 0; i < taille; i++) {
                valeurs[i] = new Cell[taille];

                for (unsigned int j = 0; j < taille; j++) {
                    Cell* newCell = new Cell();
                    valeurs[i][j] = *newCell;
                }
            }
        } else {
            throw new AutomateException("Dimension non valide");
        }
    }
	const Cell& getCell(unsigned int i) const;
	const Cell& getCell(unsigned int i, unsigned int j) const;
    void setCell(unsigned i, unsigned int j, unsigned int e);
	void setCell(unsigned i, unsigned int e);
    const unsigned int getTaille() const { return taille; }
};

std::ostream& operator<<(std::ostream& f, const Grille& g);

#endif // GRILLE_H