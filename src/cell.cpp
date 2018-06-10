#include "cell.h"

Cell& Cell::operator=(const Cell& c) {
    if (this != &c) {
        etat = c.getEtat();
    }

    return *this;
}

std::ostream& operator<<(std::ostream& f, const Cell& c) {
    unsigned int e = c.getEtat();
	f << e;
    return f;
}