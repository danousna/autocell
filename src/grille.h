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

    unsigned int getTaille() const { return taille; }
    unsigned int getDimension() const { return dimension; }
    virtual const Cell& getCell(unsigned int, unsigned int j) const = 0;
    virtual int getCellVal(unsigned int i, unsigned int j) const = 0;
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

    ~Grille1D() { delete[] valeurs; }

    const Cell& getCell(unsigned int i, unsigned int j = 0) const { return valeurs[i]; }
    int getCellVal(unsigned int i, unsigned int j = 0) const { return valeurs[i].getEtat().getValeur(); }
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

    ~Grille2D() {
        for (unsigned int i = 0; i < taille; i++) {
            delete[] valeurs[i];
        }

        delete[] valeurs;
    }

    const Cell& getCell(unsigned int i, unsigned int j) const { return valeurs[i][j]; }
    int getCellVal(unsigned int i, unsigned int j) const { return valeurs[i][j].getEtat().getValeur(); }
    void setCell(const Cell& c, unsigned i, unsigned int j);
    virtual std::ostream& afficher(std::ostream& f) const;
};

#endif // GRILLE_H