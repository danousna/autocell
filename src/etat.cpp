#include "etat.h"

Etat& Etat::operator=(const Etat& e) {
    if (this != &e) {
        valeur = e.getValeur();
        desc = e.getDesc();
    }

    return *this;
}

std::ostream& operator<<(std::ostream& f, const Etat& e) {
    f << "Valeur : " << e.getValeur() << "\n";
    f << "Description : " << e.getDesc() << "\n";
    
    return f;
}
