#ifndef WWVIEW_H
#define WWVIEW_H

#include "automateview.h"

namespace Ui {
class WWView;
}

class WWView : public AutomateView
{
    Q_OBJECT
    Ui::WWView*ui;

    int dimensions;

    // L'automate de cette view. C'est un singleton.
    AutomateWW* automate;
public:
    WWView(QWidget *parent);
    virtual ~WWView();

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
    void changeSpeed(int s);
    void changeSteps(int n);
    void randomGen();
    void symetrie();
};

#endif // WWVIEW_H
