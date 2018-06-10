#ifndef AUTOMATEGOL_H
#define AUTOMATEGOL_H

#include <string>
#include <iostream>

#include "automate.h"

class AutomateGoL:public Automate
{
public:
	AutomateGoL() :Automate(2) {};
	AutomateGoL(std::string* etats) :Automate(2, etats) {};
	void appliquerTransition(Grille* dep,Grille* dest);


};

#endif // AUTOMATEGOL_H
