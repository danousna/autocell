#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <chrono>

#include <QMainWindow>
#include <QLineEdit>
#include <QIntValidator>
#include <QTableWidget>
#include "../simulateur.h"
#include "../automate1d.h"

// TODO : AutomateManager to only get one instace of every automate.

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *ui;
    QIntValidator* zeroOneValidator;
    QLineEdit* numeroBit[8];
    int taille;
    int tailleCell;
    int steps;
private slots:
    // Chargement Fenetres
    void showFenetreAutomateElementaire();
    void showFenetreAutomateGoL();
    void showFenetreAutomateWireworld();

    void toggleCell(QTableWidgetItem* item);
    void playSimulation();

    // Sync des grilles.
    void syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, int step = 0, bool set = true);

    void viderGrille();
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupAutomateElementaireWindow();
    void setupAutomateGoLWindow();
};

#endif // MAINWINDOW_H
