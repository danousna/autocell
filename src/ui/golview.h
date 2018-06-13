#ifndef GOLVIEW_H
#define GOLVIEW_H

#include <thread>
#include <chrono>

#include <QWidget>
#include <QLineEdit>
#include <QIntValidator>
#include <QTableWidget>

#include "../simulateur.h"
#include "../automate2d.h"

namespace Ui {
class GoLView;
}

class GoLView : public QWidget
{
    Q_OBJECT
    Ui::GoLView *ui;
    int dimensions;
    int tailleCell;
    int steps;
    int stepState;
    int speed;
    bool paused;

    // L'automate de cette view. C'est un singleton.
    AutomateGoL* automate;
public:
    explicit GoLView(QWidget *parent = 0);
    ~GoLView();
private slots:
    void toggleCell(QTableWidgetItem* item);
    void next();
    void play(int startStep = 0);
    void pause();
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
};

#endif // GOLVIEW_H
