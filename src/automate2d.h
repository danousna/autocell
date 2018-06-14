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

    static AutomateGoL* singleton;
    AutomateGoL(): voisinsVivantsMin(2), voisinsVivantsMax(3) {}
    AutomateGoL(unsigned int vMin, unsigned int vMax): voisinsVivantsMin(vMin), voisinsVivantsMax(vMax) {}
public:
    static AutomateGoL* getInstance() {
        if (!singleton) {
            singleton = new AutomateGoL();
        }

        return singleton;
    }
    static AutomateGoL* getInstance(unsigned int vMin, unsigned int vMax) {
        if (!singleton) {
            singleton = new AutomateGoL(vMin, vMax);
        } else {
            if ((vMin != singleton->getVoisinsVivantsMin()) && (vMax != singleton->getVoisinsVivantsMax())) {
                delete singleton;
                singleton = new AutomateGoL(vMin, vMax);
            }
        }

        return singleton;
    }
    AutomateGoL(const AutomateGoL&) = delete;
    AutomateGoL& operator=(const AutomateGoL&) = delete;

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

    static AutomateWW* singleton;

    AutomateWW();
    /* Il n'y a qu'un constructeur parce que les règle de la modélisation sont plus strictes que pour d'autres automates */

public:
    static AutomateWW* getInstance() {
        if (!singleton) {
            singleton = new AutomateWW();
        }

        return singleton;
    }

    AutomateWW(const AutomateWW&) = delete;
    AutomateWW& operator=(const AutomateWW&) = delete;

    unsigned int getVoisinsTeteMin() const { return voisinsTeteMin; }
    unsigned int getVoisinsTeteMax() const { return voisinsTeteMax; }

    void appliquerTransition(Grille* dep, Grille* dest) const;

    int calculerSommeVoisinsTete(Grille* g, unsigned int y, unsigned int x) const;
};

std::ostream& operator<<(std::ostream& f, const AutomateWW& a);


#endif // AUTOMATEGOL_H
