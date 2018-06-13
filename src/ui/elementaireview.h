#ifndef ELEMENTAIREVIEW_H
#define ELEMENTAIREVIEW_H

#include <thread>
#include <chrono>

#include <QWidget>
#include <QLineEdit>
#include <QIntValidator>
#include <QTableWidget>

#include "../simulateur.h"
#include "../automate1d.h"

namespace Ui {
class ElementaireView;
}

class ElementaireView : public QWidget
{
    Q_OBJECT
    Ui::ElementaireView *ui;
    QIntValidator* zeroOneValidator;
    QLineEdit* numeroBit[8];
    int taille;
    int tailleCell;
    int steps;
    int stepState;
    bool paused;

    // L'automate de cette view. C'est un singleton.
    AutomateElementaire* automate;
public:
    explicit ElementaireView(QWidget *parent = 0);
    ~ElementaireView();
private slots:
    void toggleCell(QTableWidgetItem* item);
    void play(int startStep = 0);
    void pause();
    void togglePlayPause();
    void reset();
    void refreshTaille();
    void syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, int step = 0, bool set = true);
    void viderGrille();
    void drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int t, unsigned int r);
    void toggleUI();
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
};

#endif // ELEMENTAIREVIEW_H
