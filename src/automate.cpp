#include "automate.h"

// Constructeur "par défaut" créer uniquement deux etats, mort et vivant 
Automate::Automate(unsigned int d): dimension(d) {

    etatsPossibles = new std::string[2];
    etatsPossibles[0] = "mort";
    etatsPossibles[1] = "vivant";
}