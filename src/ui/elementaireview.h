#ifndef ELEMENTAIREVIEW_H
#define ELEMENTAIREVIEW_H

#include "automateview.h"

namespace Ui {
class ElementaireView;
}

class ElementaireView : public AutomateView
{
    Q_OBJECT
    Ui::ElementaireView *ui;

    QIntValidator* zeroOneValidator;
    QLineEdit* numeroBit[8];
    int taille;
public:
    ElementaireView(QWidget *parent);
    virtual ~ElementaireView();
    
    int getTaille() { return taille; }
    void save(QFile* f, bool showDialog);
    void import(QXmlStreamReader* reader);
private slots:
    void toggleCell(QTableWidgetItem* item);
    void next();
    void play(int startStep = 0);
    void togglePlayPause();
    void reset();
    void refreshTaille();
    void syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, int step = 0, bool set = true);
    void viderGrille();
    void drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int t, unsigned int r);
    void toggleUI();
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
    void changeSpeed(int s);
    void changeSteps(int n);
    void randomGen();
    void symetrie();
};

#endif // ELEMENTAIREVIEW_H
