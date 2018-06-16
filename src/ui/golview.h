#ifndef GOLVIEW_H
#define GOLVIEW_H

#include "automateview.h"

namespace Ui {
class GoLView;
}

class GoLView : public AutomateView
{
    Q_OBJECT
    Ui::GoLView *ui;

    int voisinsMin;
    int voisinsMax;
    int dimensions;
public:
    GoLView(QWidget *parent);
    virtual ~GoLView();

    int getTaille() { return dimensions; }
    void save(QFile* f, bool showDialog);
    void import(QXmlStreamReader* reader);
private slots:
    void toggleCell(QTableWidgetItem* item);
    void next();
    void play(int startStep = 0);
    void togglePlayPause();
    void reset();
    void refreshTaille();
    void syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, bool set = true);
    void viderGrille();
    void drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int n);
    void toggleUI();
    void changeVoisinsMin(int vMin);
    void changeVoisinsMax(int vMax);
    void changeSpeed(int s);
    void changeSteps(int n);
    void randomGen();
    void symetrie();
};

#endif // GOLVIEW_H
