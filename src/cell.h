#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>

#include "automateexception.h"

class Cell
{
    unsigned int etat;
public:
    Cell() {}
    Cell(const unsigned int e): etat(e) {}
    Cell(const Cell& c): etat(c.getEtat()) {}
    Cell& operator=(const Cell& c);
    void setEtat(const int e) { etat = e; }
    const unsigned int& getEtat() const { return etat; }
};

std::ostream& operator<<(std::ostream& f, const Cell& c);

#endif // CELL_H