#ifndef WWVIEW_H
#define WWVIEW_H

#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>

#include <QWidget>
#include <QLineEdit>
#include <QIntValidator>
#include <QTableWidget>
#include <QString>

#include "../simulateur.h"
#include "../automate2d.h"

namespace Ui {
class WWView;
}

class WWView : public QWidget
{
    Q_OBJECT
    Ui::WWView*ui;
    int voisinsMin;
    int voisinsMax;
    int dimensions;
    int tailleCell;
    int steps;
    int stepState;
    int speed;
    bool paused;

    // L'automate de cette view. C'est un singleton.
    AutomateWW* automate;
public:
    explicit WWView(QWidget *parent = 0);
    int getTaille() { return dimensions; }
    ~WWView();
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
