#include "automate2d.h"

void AutomateGoL::appliquerTransition(Grille* dep, Grille* dest) const {
    if (dep->getTaille() != dest->getTaille()) throw new AutoCellException("Erreur : La grille de départ n'a pas la même dimension que la grille d'arrivée.");

    unsigned int taille = dep->getTaille();
    int newValeurs[taille][taille];

    for (unsigned int i = 0; i < taille; i++) {
        for (unsigned int j = 0; j < taille; j++) {
            
            int sum = this->calculerSommeVoisins(dep, i, j);

            if (sum == voisinsVivantsMax || (sum == voisinsVivantsMin && dep->getCellVal(i, j) == 1)) {
                newValeurs[i][j] = 1;
            } else {
                newValeurs[i][j] = 0;
            }
        }
    }

    for (unsigned int i = 0; i < taille; i++) {
        for (unsigned int j = 0; j < taille; j++) {
            dest->setCell(Cell(Etat(newValeurs[i][j])), i, j);
        }
    }
}

int AutomateGoL::calculerSommeVoisins(Grille* g, unsigned int y, unsigned int x) const {
    unsigned int t = g->getTaille();
    int sum = 0;

    for (unsigned int i = y - 1; i <= y + 1; i++) {
        for (unsigned int j = x - 1; j <= x + 1; j++) {     
            if (g->getCellVal((i + t) % t, (j + t) % t) == 1) {
                sum++;
            }
        }
    }

    if (g->getCellVal(y, x) == 1) {
        sum--;
    }

    return sum;
}

std::ostream& operator<<(std::ostream& f, const AutomateGoL& a) {
    f << "Automate du Jeu de la Vie : \n";
    f << "Nombre de voisins minimum : " << a.getVoisinsVivantsMin() << "\n";
    f << "Nombre de voisins maximum : " << a.getVoisinsVivantsMax() << "\n";
    return f;
}