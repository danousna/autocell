#ifndef AUTOMATE_H
#define AUTOMATE_H

#include "automateexception.h"
#include "etat.h"

class Automate {
protected:
    unsigned int dimension;
    unsigned int etatsPossibles;
public:
    Automate(unsigned int d, unsigned int e): dimension(d), etatsPossibles(e) {}
    virtual void appliquerTransition(const Etat& dep, Etat& dest) const = 0;
};

#endif // AUTOMATE_H
