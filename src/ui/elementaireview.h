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
public:
    explicit ElementaireView(QWidget *parent = 0);
    ~ElementaireView();
private slots:
    void toggleCell(QTableWidgetItem* item);
    void playSimulation();

    // Sync des grilles.
    void syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, int step = 0, bool set = true);

    void viderGrille();
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
};

#endif // ELEMENTAIREVIEW_H
