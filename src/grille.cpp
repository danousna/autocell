#include "grille.h"

std::ostream& operator<<(std::ostream& f, const Grille& g) {
    for (unsigned int i = 0; i < g.getTaille(); i++) {
        for (unsigned int j = 0; j < g.getTaille(); j++) {
            f << g.getCell(i, j);
        }
    }

    return f;
}