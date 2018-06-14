#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->menuBar->setNativeMenuBar(false);

    // Connection des actions aux fenetres
    connect(ui->actionElementaire, SIGNAL(triggered()), this, SLOT(showFenetreAutomateElementaire()));
    connect(ui->actionGoL, SIGNAL(triggered()), this, SLOT(showFenetreAutomateGoL()));
    connect(ui->actionWireworld, SIGNAL(triggered()), this, SLOT(showFenetreAutomateWireworld()));

    // Ajout des interfaces des automates au stackedWidget.
    ui->stackedWidget->insertWidget(0, new ElementaireView);
    ui->stackedWidget->insertWidget(1, new GoLView);
}

MainWindow::~MainWindow() {
    delete ui;
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
