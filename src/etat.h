#ifndef ETAT_H
#define ETAT_H

#include <iostream>
#include <string>

#include "automateexception.h"

class Etat {
    int valeur;
    std::string desc;
public:
    Etat(): valeur(0), desc("morte") {}
    Etat(int v, std::string d): valeur(v), desc(d) {}
    Etat(int v): valeur(v), desc("pas de description") {}
    Etat(const Etat& e): valeur(e.getValeur()), desc(e.getDesc()) {}
    int getValeur() const { return valeur; }
    std::string getDesc() const { return desc; }
    Etat& operator=(const Etat& e);
};

std::ostream& operator<<(std::ostream& f, const Etat& e);

#endif // ETAT_H
