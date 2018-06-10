#include "automate1d.h"

void AutomateElementaire::appliquerTransition(Grille* dep, Grille* dest) const {
    if (dep->getTaille() != dest->getTaille()) throw new AutomateException("Erreur : La grille de départ n'a pas la même dimension que la grille d'arrivée.");

    for (unsigned int i = 0; i < dep->getTaille(); i++) {
        // Parmi les états possibles, setCell avec le bon état.
        // if (valResult == etatsPossibles[0].getValeur()) {
        //     dest->setCell(Cell(etatsPossibles[0]), i, 0);
        // } else if (valResult == etatsPossibles[1].getValeur()) {
        //     dest->setCell(Cell(etatsPossibles[1]), i, 0);
        // } else throw new AutomateException("Erreur : Etats possibles.");

        unsigned int conf = 0;
        
        if (i > 0) {
            conf += dep->getCell(i - 1, 0).getEtat().getValeur() * 4;
        }

        conf += dep->getCell(i, 0).getEtat().getValeur() * 2;
        
        if (i < dep->getTaille() - 1) {
            conf += dep->getCell(i + 1, 0).getEtat().getValeur();
        }

        dest->setCell(Cell(Etat(numeroBit[7 - conf] - '0')), i, 0);
    }
}

std::ostream& operator<<(std::ostream& f, const AutomateElementaire& a) {
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