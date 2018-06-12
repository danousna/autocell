#ifndef AUTOMATEGOL_H
#define AUTOMATEGOL_H

#include "automate.h"

class Automate2D : public Automate
{
public:
    Automate2D(): Automate(2) {}
    Automate2D(Etat* etats): Automate(2, etats) {}
    virtual void appliquerTransition(Grille* dep, Grille* dest) const = 0;
};

class AutomateGoL : public Automate2D
{
    unsigned int voisinsVivantsMin;
    unsigned int voisinsVivantsMax;
public:
    AutomateGoL(): voisinsVivantsMin(2), voisinsVivantsMax(3) {}
    AutomateGoL(unsigned int vMin, unsigned int vMax): voisinsVivantsMin(vMin), voisinsVivantsMax(vMax) {}

    unsigned int getVoisinsVivantsMin() const { return voisinsVivantsMin; }
    unsigned int getVoisinsVivantsMax() const { return voisinsVivantsMax; }
    void appliquerTransition(Grille* dep, Grille* dest) const;
    int calculerSommeVoisins(Grille* g, unsigned int y, unsigned int x) const;
};

std::ostream& operator<<(std::ostream& f, const AutomateGoL& a);

class AutomateWW : public Automate2D
{
    unsigned int voisinsTeteMin;
    unsigned int voisinsTeteMax;
public:
    AutomateWW();
    /* Il n'y a qu'un constructeur parce que les règle de la modélisation sont plus strictes que pour d'autres automates */

    unsigned int getVoisinsTeteMin() const { return voisinsTeteMin; }
    unsigned int getVoisinsTeteMax() const { return voisinsTeteMax; }
    void appliquerTransition(Grille* dep, Grille* dest) const;
    int calculerSommeVoisinsTete(Grille* g, unsigned int y, unsigned int x) const;
};

std::ostream& operator<<(std::ostream& f, const AutomateWW& a);


#endif // AUTOMATEGOL_H
