#ifndef AUTOMATE_H
#define AUTOMATE_H

#include "automateexception.h"
#include "cell.h"
#include "grille.h"
#include <string>
#include <iostream>

class Automate {
protected:
    unsigned int dimension;
    std::string* etatsPossibles;
public:
    Automate(unsigned int d);
    Automate(unsigned int d, std::string*  etats): dimension(d), etatsPossibles(etats) {}
    virtual ~Automate() { delete[] etatsPossibles; }
    unsigned int getDimension() const { return dimension; }
    const std::string* getEtatsPossibles() const { return etatsPossibles; }
    virtual void appliquerTransition(Grille* dep, Grille* dest) const = 0;
};

#endif // AUTOMATE_H
