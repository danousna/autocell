#ifndef ETAT_H
#define ETAT_H

#include <iostream>
#include <string>

#include "automateexception.h"

class Etat {
    unsigned int dimension;
    bool* valeur;
public:
    Etat(): dimension(0), valeur(nullptr) {}
    Etat(unsigned int n);
    ~Etat() { delete[] valeur; }
    Etat(const Etat& e);
    Etat& operator=(const Etat& e);
    void setCellule(unsigned int i, bool val);
    bool getCellule(unsigned int) const;
    unsigned int getDimension() const { return dimension; }
};

std::ostream& operator<<(std::ostream& f, const Etat& e);

#endif // ETAT_H
