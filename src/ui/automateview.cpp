#include "automateview.h"

AutomateView::AutomateView(QWidget *parent, int tCell, int s, int sS, int vitesse, bool p, Automate* a):
QWidget(parent), tailleCell(tCell), steps(s), stepState(sS), speed(vitesse), paused(p), automate(a) {}