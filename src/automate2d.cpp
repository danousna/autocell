#include "automate2d.h"

void AutomateGoL::appliquerTransition(Grille* dep, Grille* dest) const {
    if (dep->getTaille() != dest->getTaille()) throw new AutomateException("Erreur : La grille de départ n'a pas la même dimension que la grille d'arrivée.");

    unsigned int taille = dep->getTaille();
    int newValeurs[taille][taille];

    for (unsigned int i = 0; i < taille; i++) {
        for (unsigned int j = 0; j < taille; j++) {
            
            // Calcul de la somme
            unsigned int sum = 0;

            for (unsigned int k = i - 1; k <= j + 1; k++) {
                for (unsigned int l = j - 1; l <= j + 1; l++) {         
                    if (dep->getCellVal((k + taille) % taille, (l + taille) % taille) == 1) {
                        sum++;
                    }
                }
            }

            if (dep->getCellVal(i, j) == 1) {
                sum--;
            }

            if (sum == voisinsVivantsMax || (sum == voisinsVivantsMin && dep->getCellVal(i, j) == 1)) {
                newValeurs[i][j] = 1;
            } else {
                newValeurs[i][j] = 0;
            }

            std::cout << "(" << i << ", " << j << ") : " << sum << "\n";
        }
    }

    for (unsigned int i = 0; i < taille; i++) {
        for (unsigned int j = 0; j < taille; j++) {
            dest->setCell(Cell(Etat(newValeurs[i][j])), i, j);
        }
    }
}

std::ostream& operator<<(std::ostream& f, const AutomateGoL& a) {
    f << "Automate du Jeu de la Vie \n";
    return f;
}