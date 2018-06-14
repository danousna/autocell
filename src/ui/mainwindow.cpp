#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent), ui(new Ui::MainWindow),
elementaireAutomate(new ElementaireView), golAutomate(new GoLView) {
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
    ui->stackedWidget->insertWidget(0, elementaireAutomate);
    ui->stackedWidget->insertWidget(1, golAutomate);
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

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_5);

        out << elementaireAutomate->save();
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
