#include "automategol.h"

void AutomateGoL::appliquerTransition(Grille* dep, Grille* dest) {
	for (unsigned int i = 0; i < dep->getTaille(); i++) {
		for (unsigned int j = 0; j < dep->getTaille(); j++) {
			unsigned int conf = 0;
			if (i > 0 && j > 0)
				conf+=dep->getCell(i-1,j-1).getEtat();//gauche-haut
			if (i > 0)
				conf+=dep->getCell(i-1,j).getEtat();//haut
			if (i > 0 && i < dep->getTaille() - 1)
				conf+= dep->getCell(i-1,j+1).getEtat();//droite->haut
			if (j > 0)
				conf+=dep->getCell(i,j-1).getEtat();
			conf+= dep->getCell(i,j).getEtat();
			if (j<dep->getTaille()-1)
				conf+= dep->getCell(i,j+1).getEtat();
			if (i<dep->getTaille()-1 && j>0)
				conf+= dep->getCell(i+1,j-1).getEtat();
			if (i<dep->getTaille()-1)
				conf += dep->getCell(i+1, j).getEtat();
			if (i<dep->getTaille()-1 && j<dep->getTaille()-1)
				conf+= dep->getCell(i + 1, j+1).getEtat();

			switch (conf) {
			case 2:
				if (dep->getCell(i,j).getEtat()==1)
					dest->setCell(i, j, 1);
				else dest->setCell(i, j, 0);
				break;

			case 3:
				dest->setCell(i, j, 1);
				break;

			default:
				dest->setCell(i, j, 0);
				break;
			}

		}
	}
}
