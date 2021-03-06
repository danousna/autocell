#ifndef AUTOMATE1D_H
#define AUTOMATE1D_H

#include "automate.h"

unsigned int NumBitToNum(const std::string& num);
std::string NumToNumBit(unsigned int num);

class Automate1D : public Automate
{
public:
    Automate1D(): Automate(1) {}
    Automate1D(Etat* etats): Automate(1, etats) {}
    virtual void appliquerTransition(Grille* dep, Grille* dest) const = 0;
};

class AutomateElementaire : public Automate1D
{
    unsigned int numero;
    std::string numeroBit;

    static AutomateElementaire* singleton;
    AutomateElementaire(unsigned int n): numero(n), numeroBit(NumToNumBit(n)) {}
    AutomateElementaire(const std::string& num): numero(NumBitToNum(num)), numeroBit(num) {}
public:
    static AutomateElementaire* getInstance(unsigned int n) {
        if (!singleton) {
            singleton = new AutomateElementaire(n);
        } else {
            if (n != singleton->getNumero()) {
                delete singleton;
                singleton = new AutomateElementaire(n);
            }
        }

        return singleton;
    }
    static AutomateElementaire* getInstance(const std::string& num) {
        if (!singleton) {
            singleton = new AutomateElementaire(num);
        } else {
            if (num != singleton->getNumeroBit()) {
                delete singleton;
                singleton = new AutomateElementaire(num);
            }
        }

        return singleton;
    }
    AutomateElementaire(const AutomateElementaire&) = delete;
    AutomateElementaire& operator=(const AutomateElementaire&) = delete;

    unsigned int getNumero() const { return numero; }
    const std::string& getNumeroBit() const { return numeroBit; }
    void appliquerTransition(Grille* dep, Grille* dest) const;
};

std::ostream& operator<<(std::ostream& f, const AutomateElementaire& a);

#endif // AUTOMATE1D_H
