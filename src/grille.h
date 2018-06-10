#ifndef GRILLE_H
#define GRILLE_H

#include "cell.h"
#include "etat.h"

/*
    TODO(Natan): 
        - destructeur pour tableau dynamique
*/

class Grille
{
protected:
    unsigned int taille;
    unsigned int dimension;
public:
    Grille(unsigned int t, unsigned int d): taille(t), dimension(d) {}

    virtual ~Grille() {}
    virtual Grille* clone() const = 0;

    const unsigned int getTaille() const { return taille; }
    const unsigned int getDimension() const { return dimension; }
    virtual const Cell& getCell(unsigned int i, unsigned int j) const = 0;
    virtual void setCell(const Cell& c, unsigned i, unsigned int j) = 0;
    virtual std::ostream& afficher(std::ostream& f) const = 0;
};

std::ostream& operator<<(std::ostream& f, const Grille& g);

class Grille1D : public Grille {
    Cell* valeurs;
public:
    Grille1D(unsigned int t): Grille(t, 1) {
        valeurs = new Cell[taille];

        for (unsigned int i = 0; i < taille; i++) {
            Cell* newCell = new Cell();
            valeurs[i] = *newCell;
        }
    }

    Grille1D(const Grille1D& g): Grille(g.getTaille(), g.getDimension()) {
        valeurs = new Cell[taille];

        for (unsigned int i = 0; i < taille; i++) {
            valeurs[i] = Cell(g.getCell(i));
        }
    }

    Grille1D* clone() const { return new Grille1D(*this); }

    const Cell& getCell(unsigned int i, unsigned int j = 0) const { return valeurs[i]; }
    void setCell(const Cell& c, unsigned i, unsigned int j = 0);
    virtual std::ostream& afficher(std::ostream& f) const;
};

class Grille2D : public Grille {
    Cell** valeurs;
public:
    Grille2D(unsigned int t): Grille(t, 2) {
        valeurs = new Cell*[taille];

        for (unsigned int i = 0; i < taille; i++) {
            valeurs[i] = new Cell[taille];

            for (unsigned int j = 0; j < taille; j++) {
                Cell* newCell = new Cell();
                valeurs[i][j] = *newCell;
            }
        }
    }

    Grille2D(const Grille2D& g): Grille(g.getTaille(), g.getDimension()) {
        valeurs = new Cell*[taille];

        for (unsigned int i = 0; i < taille; i++) {
            valeurs[i] = new Cell[taille];

            for (unsigned int j = 0; j < taille; j++) {
                valeurs[i][j] = Cell(g.getCell(i, j));
            }
        }
    }

    Grille2D* clone() const { return new Grille2D(*this); }

    const Cell& getCell(unsigned int i, unsigned int j) const { return valeurs[i][j]; }
    void setCell(const Cell& c, unsigned i, unsigned int j);
    virtual std::ostream& afficher(std::ostream& f) const;
};

#endif // GRILLE_H