#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include "elementaireview.h"

#include "../simulateur.h"
#include "../automate1d.h"

// TODO : AutomateManager to only get one instance of every automate.

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *ui;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    // Chargement Fenetres
    void showFenetreAutomateElementaire();
    void showFenetreAutomateGoL();
    void showFenetreAutomateWireworld();
};

#endif // MAINWINDOW_H
