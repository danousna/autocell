#include "automate1d.h"

void Automate1D::appliquerTransition(const Etat& dep, Etat& dest) const {
    if (dep.getDimension() != dest.getDimension()) {
        dest = dep;
    }

    for (unsigned int i = 0; i < dep.getDimension(); i++) {
        unsigned int conf=0;
        
        if (i > 0) {
            conf += dep.getCellule(i - 1) * 4;
        }

        conf += dep.getCellule(i) * 2;
        if (i < dep.getDimension() - 1) {
            conf += dep.getCellule(i + 1);
        }
        dest.setCellule(i, numeroBit[7-conf] - '0');
    }
}

std::ostream& operator<<(std::ostream& f, const Automate1D& a) {
    f << a.getNumero() << " : " << a.getNumeroBit() << "\n";
    return f;
}


// Fonctions helpers

unsigned int NumBitToNum(const std::string& num) {
    if (num.size() != 8) throw AutomateException("Numero d'automate indefini");
    int puissance = 1;
    unsigned int numero = 0;
    for (int i = 7; i >= 0; i--) {
        if (num[i] == '1') {
            numero += puissance;
        } else if (num[i] != '0') throw AutomateException("Numero d'automate indefini");
        puissance *= 2;
    }
    return numero;
}

std::string NumToNumBit(unsigned int num) {
    std::string numeroBit;
    if (num > 256) throw AutomateException("Numero d'automate indefini");
    unsigned int p = 128;
    int i = 7;
    while (i >= 0) {
        if (num >= p) {
            numeroBit.push_back('1');
            num -= p;
        }
        else {
            numeroBit.push_back('0');
        }
        i--;
        p = p / 2;
    }
    return numeroBit;
}