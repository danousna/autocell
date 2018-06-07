#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>

#include "etat.h"
#include "automateexception.h"

class Cell
{
    Etat etat;
public:
    Cell() {}
    Cell(const Etat& e): etat(e) {}
    Cell(const Cell& c): etat(c.getEtat()) {}
    Cell& operator=(const Cell& c);
    void setEtat(const Etat& e) { etat = e; }
    const Etat& getEtat() const { return etat; }
};

#endif // CELL_H