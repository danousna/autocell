#include "cell.h"

Cell& Cell::operator=(const Cell& c) {
    if (this != &c) {
        etat = c.getEtat();
    }

    return *this;
}