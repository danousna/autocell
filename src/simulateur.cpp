#include "simulateur.h"

Simulateur::Simulateur(const Automate& a, unsigned int t, unsigned int b): 
automate(a), taille(t), buffer(b), rang(0) {
    unsigned int dimension = a.getDimension();
    const Etat* etatsPossibles = a.getEtatsPossibles();

    // On crée un pointeur vers une liste de pointeurs de taille buffer.
    // Cette liste contient des pointeurs vers des tableaux de pointeurs de
    // taille NMAX par NMAX. Ces tableaux contiennent des pointeurs vers
    // des objets Cell.

    if (dimension == 2) {
        grilles = new Grille[buffer];

        for (unsigned int i = 0; i < buffer; i++) {
            grilles[i] = new Cell*[taille];

            for (unsigned int j = 0; j < taille; j++) {
                grilles[i][j] = new Cell[taille];

                for (unsigned int k = 0; k < taille; k++) {
                    // TODO(Natan): Voir si on crée vraiment avec le premier
                    // etat possible.
                    Cell* newCell = new Cell(etatsPossibles[0]);
                    grilles[i][j][k] = *newCell; // PK CA MARCHE ?!
                }
            }
        }
    } else {
        grilles = new Cell**[buffer];

        for (unsigned int i = 0; i < taille; i++) {
            grilles[i] = new Cell*[taille];

            for (unsigned int j = 0; j < taille; j++) {
                // TODO(Natan): Voir si on crée vraiment avec le premier
                // etat possible.
                
            }
        }
    }
}

void Simulateur::build(unsigned int r) {
    // Implement me.
}

void Simulateur::setGrilleInitiale(const Cell** depart) {
    grilleInitiale = depart;
    reset();
}

void Simulateur::reset() {
    if (grilleInitiale == nullptr) throw AutomateException("Grille initiale indefinie.");
    build(0);
    grilles[0] = const_cast<Cell**>(grilleInitiale);
    rang = 0;
}

void Simulateur::next() {
    if (grilleInitiale == nullptr) throw AutomateException("Grille initiale indefinie.");
    rang++;
    build(rang % buffer);
    automate.appliquerTransition(grilles[(rang - 1) % buffer], grilles[rang % buffer]);
}

void Simulateur::run(unsigned int nb_steps) {
    for (unsigned int i = 0; i < nb_steps; i++) {
        next();
    }
}

Cell** Simulateur::dernier() const {
    return grilles[rang % buffer];
}

Simulateur::~Simulateur() {
    for (unsigned int i = 0; i < buffer; i++) {
        for (unsigned int j = 0; j < taille; j++) {
            delete grilles[i][j];
        }
    }
    delete[] grilles;
}
