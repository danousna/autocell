#include "grille.h"

std::ostream& operator<<(std::ostream& f, const Grille& g) {
    for (unsigned int i = 0; i < g.getTaille(); i++) {
        for (unsigned int j = 0; j < g.getTaille(); j++) {
            f << g.getCell(i, j);
        }
        f << std::endl;
    }

    return f;
}

void Grille::setCell(unsigned i, unsigned int j,unsigned int e) {
	if (this->dimension == 2) {
		if (i < this->getTaille() && j < this->getTaille()) {
			this->valeurs[i][j].setEtat(e);
		}
	}
}

void Grille::setCell(unsigned i, unsigned int e) {
	if (this->dimension == 1) {
		if (i < this->getTaille()) {
			this->valeurs[i][0].setEtat(e);
		}
	}
}

const Cell& Grille::getCell(unsigned int i) const {
	if (this->dimension == 1)
		return valeurs[i][0];
}

const Cell& Grille::getCell(unsigned int i, unsigned int j) const{
	if (this->dimension == 2 && valeurs[i] != nullptr)
		return valeurs[i][j];
}