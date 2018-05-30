#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include "automate1d.h"
#include "etat.h"

class Simulateur {
    const Automate1D& automate;
    Etat** etats;
    const Etat* depart;
    unsigned int nbMaxEtats;
    unsigned int rang;
    void build(unsigned int c);
    Simulateur(const Simulateur& s);
    Simulateur& operator=(const Simulateur& s);
public:
    Simulateur(const Automate1D& a, unsigned int buffer = 2);
    Simulateur(const Automate1D& a, const Etat& dep, unsigned int buffer = 2);
    void setEtatDepart(const Etat& e);
    void run(unsigned int nbSteps); // génère les n prochains états
    void next(); // génère le prochain état
    const Etat& dernier() const;
    unsigned int getRangDernier() const { return rang; }
    void reset(); // revenir à l'état de départ
    ~Simulateur();

    class iterator {
        friend class Simulateur;
        Simulateur* sim;
        int i;
        iterator(Simulateur* s): sim(s), i(s->rang) {}
        iterator(Simulateur* s, int dep) :sim(s), i(dep) {}
    public:
        iterator():sim(nullptr),i(0) {}
        iterator& operator++() {
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
            return *this;
        }
        Etat& operator*() const {
            return *sim->etats[i%sim->nbMaxEtats];
        }
        bool operator!=(iterator it) const { return sim != it.sim || i != it.i; }
    };

    iterator begin() {
        return iterator(this);
    }
    iterator end() {
        if (rang < nbMaxEtats) return iterator(this, -1); 
        else return iterator(this, rang - nbMaxEtats);
    }

    class const_iterator {
        friend class Simulateur;
        const Simulateur* sim;
        int i;
        const_iterator(const Simulateur* s) :sim(s), i(s->rang) {}
        const_iterator(const Simulateur* s, int dep) :sim(s), i(dep) {}
    public:
        const_iterator():sim(nullptr),i(0) {}
        const_iterator& operator++() {
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
            return *this;
        }
        const Etat& operator*() const {
            return *sim->etats[i%sim->nbMaxEtats];
        }
        bool operator!=(const_iterator it) const { return sim != it.sim || i != it.i; }
    };

    const_iterator begin() const { 
        return const_iterator(this); 
    }
    const_iterator end() const { 
        if (rang < nbMaxEtats) return const_iterator(this, -1); 
        else return const_iterator(this, rang - nbMaxEtats); 
    }
};

#endif // SIMULATEUR_H
