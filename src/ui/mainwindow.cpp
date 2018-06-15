#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent), ui(new Ui::MainWindow),
elementaireAutomate(new ElementaireView), golAutomate(new GoLView), wwAutomate(new WWView) {
    ui->setupUi(this);

    ui->menuBar->setNativeMenuBar(false);

    // Connection des save/import aux dialogues box.
    connect(ui->actionImporter, SIGNAL(triggered()), this, SLOT(showImportDialog()));
    connect(ui->actionEnregistrer, SIGNAL(triggered()), this, SLOT(showSaveDialog()));

    // Connection des actions aux fenetres
    connect(ui->actionElementaire, SIGNAL(triggered()), this, SLOT(showFenetreAutomateElementaire()));
    connect(ui->actionGoL, SIGNAL(triggered()), this, SLOT(showFenetreAutomateGoL()));
    connect(ui->actionWireworld, SIGNAL(triggered()), this, SLOT(showFenetreAutomateWireworld()));

    // Ajout des interfaces des automates au stackedWidget.
    ui->stackedWidget->insertWidget(0, new ElementaireView);
    ui->stackedWidget->insertWidget(1, new GoLView);
    ui->stackedWidget->insertWidget(2, new WWView);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showImportDialog() {

}

void MainWindow::showSaveDialog() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Enregistrer l'automate"), "", tr("Fichier XML (*.xml)"));

    if (filename.isEmpty()) {
        return;
    } else {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Impossible d'ouvrir le fichier."), file.errorString());
            return;
        }

        if (ui->stackedWidget->currentIndex() == 0) {
            elementaireAutomate->save(&file);
        } 
        else if (ui->stackedWidget->currentIndex() == 1) {
            golAutomate->save(&file);
        } 
        else if (ui->stackedWidget->currentIndex() == 2) {
            //wwAutomate->save(&file);
            return;
        } 
        else {
            return;
        }
    }
}

void MainWindow::showFenetreAutomateElementaire() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showFenetreAutomateGoL() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showFenetreAutomateWireworld() {
    ui->stackedWidget->setCurrentIndex(2);
}
