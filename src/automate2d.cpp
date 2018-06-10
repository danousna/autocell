#include "automate2d.h"

void AutomateGoL::appliquerTransition(Grille* dep, Grille* dest) const {
    if (dep->getTaille() != dest->getTaille()) throw new AutomateException("Erreur : La grille de départ n'a pas la même dimension que la grille d'arrivée.");

    for (unsigned int i = 0; i < dep->getTaille(); i++) {
        for (unsigned int j = 0; j < dep->getTaille(); j++) {

            // Calcul somme des voisins.
            int sum = 0;
            if ((i > 0) && (i < dep->getTaille() - 1) && (j > 0) && (j < dep->getTaille() - 1)) {
                sum = dep->getCellVal(i, j - 1) +
                        dep->getCellVal(i - 1, j - 1) +
                        dep->getCellVal(i - 1, j) +
                        dep->getCellVal(i - 1, j + 1) +
                        dep->getCellVal(i, j + 1) +
                        dep->getCellVal(i + 1, j + 1) +
                        dep->getCellVal(i + 1, j) +
                        dep->getCellVal(i + 1, j - 1);
            }

            // Elle nait
            if (sum == voisinsVivantsMax && !dep->getCellVal(i, j)) {
                dest->setCell(Cell(Etat(1, "vivante")), i, j);
            }
            // Elle meurt 
            else if ((sum < voisinsVivantsMin || sum > voisinsVivantsMax) && dep->getCellVal(i, j)) {
                dest->setCell(Cell(Etat(0, "morte")), i, j);
            }
            // Else, elle ne change pas d'état.
            else {
                dest->setCell(Cell(dep->getCell(i, j)), i, j);
            }
        }
    }
}

std::ostream& operator<<(std::ostream& f, const AutomateGoL& a) {
    f << "Automate du Jeu de la Vie \n";
    return f;
}