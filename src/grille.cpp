#include "grille.h"

std::ostream& operator<<(std::ostream& f, const Grille& g) {
    return g.afficher(f);
}

void Grille1D::setCell(unsigned i, unsigned int j) {

}

std::ostream& Grille1D::afficher(std::ostream& f) const {
    for (unsigned int i = 0; i < taille; i++) {
        f << valeurs[i];
    }
    f << std::endl;

    return f;
}

void Grille2D::setCell(unsigned i, unsigned int j) {

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