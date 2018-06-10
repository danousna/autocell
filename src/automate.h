#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <string>
#include <iostream>

#include "automateexception.h"
#include "etat.h"
#include "cell.h"
#include "grille.h"

class Automate {
protected:
    unsigned int dimension;
    Etat* etatsPossibles;
public:
    Automate(unsigned int d);
    Automate(unsigned int d, Etat* etats): dimension(d), etatsPossibles(etats) {}
    virtual ~Automate() { delete[] etatsPossibles; }
    unsigned int getDimension() const { return dimension; }
    const Etat* getEtatsPossibles() const { return etatsPossibles; }
    virtual void appliquerTransition(Grille* dep, Grille* dest) const = 0;
};

#endif // AUTOMATE_H
