#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include "elementaireview.h"
#include "golview.h"
#include "wwview.h"

#include "../simulateur.h"
#include "../automate1d.h"

// TODO : Si on quitte, enregistrer config dans un xml qq part.
// TODO : Enregistrer, importer à faire.

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
