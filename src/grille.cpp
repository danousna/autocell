#include "grille.h"

std::ostream& operator<<(std::ostream& f, const Grille& g) {
    return g.afficher(f);
}

void Grille1D::setCell(const Cell& c, unsigned i, unsigned int j) {
    if (i > taille) throw new AutoCellException("L'index est plus grand que la taille de la grille.");
    valeurs[i] = c;
}

std::ostream& Grille1D::afficher(std::ostream& f) const {
    for (unsigned int i = 0; i < taille; i++) {
        f << valeurs[i];
    }

    return f;
}

void Grille2D::setCell(const Cell& c, unsigned i, unsigned int j) {
    if (i > taille || j > taille) throw new AutoCellException("L'index est plus grand que la taille de la grille.");
    valeurs[i][j] = c;
}

std::ostream& Grille2D::afficher(std::ostream& f) const {
    for (unsigned int i = 0; i < taille; i++) {
        for (unsigned int j = 0; j < taille; j++) {
            f << valeurs[i][j];
        }
        f << std::endl;
    }

    return f;
}