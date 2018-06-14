#include "automate2d.h"

AutomateGoL* AutomateGoL::singleton = nullptr;

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


AutomateWW* AutomateWW::singleton = nullptr;

AutomateWW::AutomateWW(): voisinsTeteMin(1), voisinsTeteMax(2) {
    const Etat vide(0, "vide");
    const Etat conducteur(1, "conductrice");
    const Etat tete(2, "tête d'élection");
    const Etat queue(3, "queue d'électron");

    etatsPossibles = new Etat[4];
    etatsPossibles[0] = vide;
    etatsPossibles[1] = conducteur;
    etatsPossibles[1] = tete;
    etatsPossibles[1] = queue;
}

void AutomateWW::appliquerTransition(Grille* dep, Grille* dest) const {
    if (dep->getTaille() != dest->getTaille()) throw new AutoCellException("Erreur : La grille de départ n'a pas la même dimension que la grille d'arrivée.");

    unsigned int taille = dep->getTaille();
    int newValeurs[taille][taille];

    for (unsigned int i = 0; i < taille; i++) {
        for (unsigned int j = 0; j < taille; j++) {

            /* Rappel :
             * 0 = cellule vide
             * 1 = cellule conductrice
             * 2 = tête d'électron
             * 3 = queue d'électron */

            if (dep->getCellVal(i, j) == 0) {
                newValeurs[i][j] = 0;
                break;
            }

            if (dep->getCellVal(i, j) == 2) {
                newValeurs[i][j] = 3;
                break;
            }

            if (dep->getCellVal(i, j) == 3) {
                newValeurs[i][j] = 1;
                break;
            }

            if (dep->getCellVal(i, j) != 1) throw new AutoCellException("Erreur : une cellule a une valeur illégale.");

            int sum = this->calculerSommeVoisinsTete(dep, i, j);

            if (sum <= voisinsTeteMax && sum >= voisinsTeteMin) {
                newValeurs[i][j] = 2;
            } else {
                newValeurs[i][j] = 1;
            }
        }
    }

    for (unsigned int i = 0; i < taille; i++) {
        for (unsigned int j = 0; j < taille; j++) {
            dest->setCell(Cell(Etat(newValeurs[i][j])), i, j);
        }
    }
}

int AutomateWW::calculerSommeVoisinsTete(Grille* g, unsigned int y, unsigned int x) const {
    unsigned int t = g->getTaille();
    int sum = 0;

    /* Rappel : une tete d'électron est codée par la valeur 2 */
    for (unsigned int i = y - 1; i <= y + 1; i++) {
        for (unsigned int j = x - 1; j <= x + 1; j++) {
            if (g->getCellVal((i + t) % t, (j + t) % t) == 2) {
                sum++;
            }
        }
    }

    if (g->getCellVal(y, x) == 2) {
        sum--;
    }

    return sum;
}

std::ostream& operator<<(std::ostream& f, const AutomateWW& a) {
    f << "Automate Wireworld : \n";
    f << "Une cellule vide reste vide.\n";
    f << "Une tête d'électron devient une queue d'électron.\n";
    f << "Une queue d'électron devient un conducteur.\n";
    f << "Un conducteur devient une tête d'électron si " << a.getVoisinsTeteMin() << " ou " << a.getVoisinsTeteMax() << " des cellules voisines sont des têtes d'électron.\n";

    return f;
}
