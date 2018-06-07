#include "automate.h"

// Constructeur "par défaut" créer uniquement deux etats, mort et vivant 
Automate::Automate(unsigned int d): dimension(d) {
    const Etat mort(0, "morte");
    const Etat vivant(1, "vivante");

    etatsPossibles = new Etat[2];
    etatsPossibles[0] = mort;
    etatsPossibles[1] = vivant;
}