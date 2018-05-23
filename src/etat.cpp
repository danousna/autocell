#include "etat.h"

Etat::Etat(unsigned int n): dimension(n), valeur(new bool [n]) {
    for (unsigned int i = 0; i < n; i++) {
        valeur[i] = false;
    }
}

void Etat::setCellule(unsigned int i, bool val) {
    if (i >= dimension) throw AutomateException("Erreur Cellule");
    valeur[i] = val;
}

bool Etat::getCellule(unsigned int i) const {
    if (i >= dimension) throw AutomateException("Erreur Cellule");
    return valeur[i];
}

Etat::Etat(const Etat& e): dimension(e.dimension), valeur(new bool[e.dimension])
{
    for (unsigned int i = 0; i < dimension; i++) {
        valeur[i] = e.valeur[i];
    }
}

std::ostream& operator<<(std::ostream& f, const Etat& e) {
    for (unsigned int i = 0; i < e.getDimension(); i++) {
        if (e.getCellule(i)) {
            f << char(178);
        } else {
            f << " ";
        }
    }
    return f;
}

Etat& Etat::operator=(const Etat& e) {
    if (this != &e) {
        if (dimension != e.dimension) {
            bool* newvaleur = new bool[e.dimension];
            for (unsigned int i = 0; i < e.dimension; i++) {
                newvaleur[i] = e.valeur[i];
            }
            bool* old = valeur;
            valeur = newvaleur;
            dimension = e.dimension;
            delete[] old;
        } else {
            for (unsigned int i = 0; i < e.dimension; i++) {
                valeur[i] = e.valeur[i];
            }
        }
    }
    return *this;
}
