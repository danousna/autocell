#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include "automate1d.h"
#include "etat.h"
#include "cell.h"

class Simulateur {
    const Automate& automate;
    Cell*** grilles;
    const Cell** grilleInitiale;
    unsigned int taille;
    unsigned int buffer;
    unsigned int rang;
    void build(unsigned int r);
    Simulateur(const Simulateur& s);
    Simulateur& operator=(const Simulateur& s);
public:
    Simulateur(const Automate& a, unsigned int t, unsigned int b = 2);

    Simulateur(const Automate& a, const Cell** depart, unsigned int t, unsigned int b = 2):
    automate(a), grilles(nullptr), grilleInitiale(nullptr), taille(t), buffer(b), rang(0) {
        // Implement me.
    }

    ~Simulateur();

    void setGrilleInitiale(const Cell** depart);
    void run(unsigned int nbSteps);
    void next();
    Cell** dernier() const;
    unsigned int getRangDernier() const { return rang; }
    void reset();
    
    class iterator {
        friend class Simulateur;
        Simulateur* sim;
        int i;
        iterator(Simulateur* s): sim(s), i(s->rang) {}
        iterator(Simulateur* s, int dep): sim(s), i(dep) {}
    public:
        iterator(): sim(nullptr), i(0) {}
        
        iterator& operator++() {
            i--;

            if (i == -1 && sim->rang >= sim->buffer) {
                i = sim->buffer - 1;
            }

            return *this;
        }

        Cell* operator*() const {
            return *sim->grilles[i % sim->buffer];
        }

        bool operator!=(iterator it) const { return sim != it.sim || i != it.i; }
    };

    iterator begin() {
        return iterator(this);
    }
    iterator end() {
        if (rang < buffer) {
            return iterator(this, -1);
        } else { 
            return iterator(this, rang - buffer);
        }
    }

    class const_iterator {
        friend class Simulateur;
        const Simulateur* sim;
        int i;
        const_iterator(const Simulateur* s): sim(s), i(s->rang) {}
        const_iterator(const Simulateur* s, int dep): sim(s), i(dep) {}
    public:
        const_iterator(): sim(nullptr), i(0) {}

        const_iterator& operator++() {
            i--;

            if (i == -1 && sim->rang >= sim->buffer) {
                i = sim->taille - 1;
            }

            return *this;
        }

        const Cell* operator*() const {
            return *sim->grilles[i % sim->buffer];
        }

        bool operator!=(const_iterator it) const { return sim != it.sim || i != it.i; }
    };

    const_iterator begin() const { 
        return const_iterator(this); 
    }

    const_iterator end() const { 
        if (rang < buffer) {
            return const_iterator(this, -1); 
        } else {
            return const_iterator(this, rang - buffer); 
        }
    }
};

#endif // SIMULATEUR_H
