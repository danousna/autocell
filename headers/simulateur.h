#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include "automate.h"
#include "etat.h"

class Simulateur {
    const Automate& automate;
    Etat** etats;
    const Etat* depart;
    unsigned int nbMaxEtats;
    unsigned int rang;
    void build(unsigned int c);
    Simulateur(const Simulateur& s);
    Simulateur& operator=(const Simulateur& s);
public:
    Simulateur(const Automate& a, unsigned int buffer = 2);
    Simulateur(const Automate& a, const Etat& dep, unsigned int buffer = 2);
    void setEtatDepart(const Etat& e);
    void run(unsigned int nbSteps); // génère les n prochains états
    void next(); // génère le prochain état
    const Etat& dernier() const;
    unsigned int getRangDernier() const { return rang; }
    void reset(); // revenir à l'état de départ
    ~Simulateur();
    friend class Iterator;
    class Iterator {
        friend class Simulateur;
        Simulateur* sim;
        int i;
        Iterator(Simulateur* s):sim(s), i(s->rang){}
    public:
        Iterator():sim(nullptr),i(0) {}
        bool isDone() const {
            return sim == nullptr ||
                   (i == -1 && sim->rang<sim->nbMaxEtats) ||
                   i == sim->rang - sim->nbMaxEtats;
        }
        void next() {
            if (isDone())
                throw AutomateException("error, next on an iterator which is done");
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
        }
        Etat& current() const {
            if (isDone())
                throw AutomateException("error, indirection on an iterator which is done");
            return *sim->etats[i%sim->nbMaxEtats];
        }
    };
    Iterator getIterator() {
        return Iterator(this);
    }
    class ConstIterator {
        friend class Simulateur;
        const Simulateur* sim;
        int i;
        ConstIterator(const Simulateur* s) :sim(s), i(s->rang) {}
    public:
        ConstIterator():sim(nullptr),i(0) {}
        bool isDone() const {
            return sim==nullptr || (i == -1 && sim->rang<sim->nbMaxEtats) ||
                i== sim->rang - sim->nbMaxEtats;
        }
        void next() {
            if (isDone())
                throw AutomateException("error, next on an iterator which is done");
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
        }
        const Etat& current() const {
            if (isDone())
                throw AutomateException("error, indirection on an iterator which is done");
            return *sim->etats[i%sim->nbMaxEtats];
        }
    };
    ConstIterator getIterator() const{
        return ConstIterator(this);
    }
    ConstIterator getConstIterator() const {
        return ConstIterator(this);
    }

    class iterator {
        friend class Simulateur;
        Simulateur* sim;
        int i;
        iterator(Simulateur* s) :sim(s), i(s->rang) {}
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

    iterator begin() {	return iterator(this); }
    iterator end() { if (rang < nbMaxEtats) return iterator(this, -1); else return iterator(this, rang - nbMaxEtats);  }

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

    const_iterator begin() const { return const_iterator(this); }
    const_iterator end() const { if (rang < nbMaxEtats) return const_iterator(this, -1); else return const_iterator(this, rang - nbMaxEtats); }
    const_iterator cbegin() const { return const_iterator(this); }
    const_iterator cend() const { if (rang < nbMaxEtats) return const_iterator(this, -1); else return const_iterator(this, rang - nbMaxEtats); }
};

#endif // SIMULATEUR_H
