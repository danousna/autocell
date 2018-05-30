#ifndef AUTOMATE1D_H
#define AUTOMATE1D_H

#include <string>
#include <iostream>

#include "automate.h"

unsigned int NumBitToNum(const std::string& num);
std::string NumToNumBit(unsigned int num);

class Automate1D : public Automate
{
    unsigned int numero;
    std::string numeroBit;
public:
    Automate1D(unsigned int d, unsigned int e, unsigned int n): Automate(d, e), numero(n), numeroBit(NumToNumBit(n)) {};
    Automate1D(unsigned int d, unsigned int e, const std::string& num): Automate(d, e), numero(NumBitToNum(num)), numeroBit(num) {};
    unsigned int getNumero() const { return numero; }
    const std::string& getNumeroBit() const { return numeroBit; }
    void appliquerTransition(const Etat& dep, Etat& dest) const;
};

std::ostream& operator<<(std::ostream& f, const Automate1D& t);

#endif // AUTOMATE1D_H
