#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <string>
#include <iostream>

#include "automateexception.h"
#include "etat.h"

class Automate {
    unsigned short int numero;
    std::string numeroBit;
    Automate(unsigned short int num);
    Automate(const std::string& num);
    ~Automate() {}
    Automate(const Automate& a);
    Automate& operator=(const Automate& a);
    friend class AutomateManager;
public:
    unsigned short int getNumero() const { return numero; }
    const std::string& getNumeroBit() const { return numeroBit; }
    void appliquerTransition(const Etat& dep, Etat& dest) const;
};

std::ostream& operator<<(std::ostream& f, const Automate& t);
short unsigned int NumBitToNum(const std::string& num);
std::string NumToNumBit(short unsigned int num);

#endif // AUTOMATE_H
