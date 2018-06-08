#include "simulateur.h"

Simulateur::Simulateur(const Automate& a, unsigned int t, unsigned int b): 
automate(a), taille(t), buffer(b), rang(0) {
    unsigned int dimension = a.getDimension();
    const Etat* etatsPossibles = a.getEtatsPossibles();

    // On crée un pointeur vers une liste de pointeurs de taille buffer.
    // Cette liste contient des pointeurs vers des tableaux de pointeurs de
    // taille NMAX par NMAX. Ces tableaux contiennent des pointeurs vers
    // des objets Cell.

    grilles = new Grille*[buffer];
    grilles[0] = new Grille(taille, dimension);
    grilles[1] = new Grille(taille, dimension);
}

void Simulateur::build(unsigned int r) {
    // Implement me.
}

void Simulateur::setGrilleInitiale(const Grille* depart) {
    grilleInitiale = depart;
    reset();
}

void Simulateur::reset() {
    if (grilleInitiale == nullptr) throw AutomateException("Grille initiale indefinie.");
    build(0);
    grilles[0] = const_cast<Grille*>(grilleInitiale);
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

Grille& Simulateur::dernier() const {
    return *grilles[rang % buffer];
}

Simulateur::~Simulateur() {
    for (unsigned int i = 0; i < buffer; i++) {
        delete grilles[i];
    }

    delete[] grilles;
}
