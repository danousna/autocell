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
// TODO : Sync two ways ! Automate->TableWiget && TableWidget->Automate.

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

    // Sync de la TableWidget avec la grille de l'automate.
    void syncGrilles(QTableWidget* grilleQT, Grille* grilleAutomate, int step = 0);

    // Sync de la grille de l'automate avec la TableWidget.
    void syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT);

    void viderGrille();
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    class AutomateGoLWindow {

    };

    class AutomateElementaireWindow {
        
    };

    class AutomateWireworldWindow {
        
    };
};

#endif // MAINWINDOW_H
