#include "simulateur.h"

Simulateur::Simulateur(const Automate& a, unsigned int t, unsigned int b): 
automate(a), taille(t), buffer(b), rang(0) {
    unsigned int dimension = a.getDimension();
    // const Etat* etatsPossibles = a.getEtatsPossibles();

    // On crée un pointeur vers une liste de pointeurs de taille buffer.
    // Cette liste contient des pointeurs vers des tableaux de pointeurs de
    // taille NMAX par NMAX. Ces tableaux contiennent des pointeurs vers
    // des objets Cell.

    grilles = new Grille*[buffer];

    if (dimension == 1) {
        grilles[0] = new Grille1D(taille);
        grilles[1] = new Grille1D(taille);
    } else {
        grilles[0] = new Grille2D(taille);
        grilles[1] = new Grille2D(taille);
    }
}

Simulateur::Simulateur(const Automate& a, const Grille& depart, unsigned int t, unsigned int b):
automate(a), grilleInitiale(depart.clone()), taille(t), buffer(b), rang(0) {
    unsigned int dimension = a.getDimension();
    // const Etat* etatsPossibles = a.getEtatsPossibles();

    grilles = new Grille*[buffer];

    if (dimension == 1) {
        grilles[0] = depart.clone();
        grilles[1] = new Grille1D(taille);
    } else {
        grilles[0] = depart.clone();
        grilles[1] = new Grille2D(taille);
    }
}

void Simulateur::setGrilleInitiale(const Grille* depart) {
    grilleInitiale = depart;
    reset();
}

void Simulateur::reset() {
    if (grilleInitiale == nullptr) throw AutomateException("Grille initiale indefinie.");
    grilles[0] = const_cast<Grille*>(grilleInitiale);
    rang = 0;
}

void Simulateur::next() {
    if (grilleInitiale == nullptr) throw AutomateException("Grille initiale indefinie.");
    rang++;
    automate.appliquerTransition(grilles[(rang - 1) % buffer], grilles[rang % buffer]);
}

void Simulateur::run(unsigned int nb_steps) {
    for (unsigned int i = 0; i < nb_steps; i++) {
        std::cout << dernier() << "\n";
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

std::ostream& operator<<(std::ostream& f, const Simulateur& s) {
    f << s.dernier();
    return f;
}
